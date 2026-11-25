/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/wiiu/net_wiiu.c -- UDP/IPv4 over nsysnet, since wut has no if.h/ifaddrs.h
 * for qcommon/net_ip.c to build against. One socket, IPv4 only, no excuses.
 */

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

#include <errno.h>
#include <malloc.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <nsysnet/_socket.h>
#include <nn/ac.h>
#include <nn/nets2/somemopt.h>
#include <coreinit/thread.h>

static int         ip_socket = -1;
static qboolean    networkingEnabled = qfalse;
static qboolean    ac_connected = qfalse;

static cvar_t      *net_ip;
static cvar_t      *net_port;

/* ---------------------------------------------------------------------- */

/* IOSU wants app-donated socket memory or networking just doesn't work; 3 MB via
 * somemopt on a detached thread, because REQUEST_INIT blocks until nsysnet dies. */
#define NET_MEMORY_SIZE 0x300000

static OSThread net_memoryThread;

static int NET_MemoryThread(int argc, void *argv)
{
	void *mem = memalign(0x40, NET_MEMORY_SIZE);
	if (!mem)
		return 0;
	somemopt(SOMEMOPT_REQUEST_INIT, mem, NET_MEMORY_SIZE, SOMEMOPT_FLAGS_NONE);
	free(mem); /* reached only once nsysnet shuts down */
	return 0;
}

static void NET_MemoryThreadDeallocator(OSThread *thread, void *stack)
{
	free(stack);
}

static void NET_DonateSocketMemory(void)
{
	const int stackSize = 128 * 1024;
	uint8_t *stack = (uint8_t *)memalign(16, stackSize);

	if (!stack)
		return;

	if (!OSCreateThread(&net_memoryThread, NET_MemoryThread, 0, NULL,
	                    stack + stackSize, stackSize, 0x10,
	                    OS_THREAD_ATTRIB_AFFINITY_ANY | OS_THREAD_ATTRIB_DETACHED))
	{
		free(stack);
		return;
	}

	OSSetThreadName(&net_memoryThread, "NetMemory");
	OSSetThreadDeallocator(&net_memoryThread, NET_MemoryThreadDeallocator);
	OSResumeThread(&net_memoryThread);

	if (somemopt(SOMEMOPT_REQUEST_WAIT_FOR_INIT, NULL, 0, SOMEMOPT_FLAGS_NONE) == -1)
		Com_Printf("NET_DonateSocketMemory: somemopt wait failed\n");
	else
		Com_Printf("NET_DonateSocketMemory: donated %d KB to nsysnet\n",
		           NET_MEMORY_SIZE / 1024);
}

/* ---------------------------------------------------------------------- */

static void NetadrToSockadr(const netadr_t *a, struct sockaddr_in *s)
{
	Com_Memset(s, 0, sizeof(*s));
	s->sin_family = AF_INET;
	if (a->type == NA_BROADCAST)
		s->sin_addr.s_addr = INADDR_BROADCAST;
	else
		Com_Memcpy(&s->sin_addr, a->ip, sizeof(s->sin_addr));
	s->sin_port = a->port;
}

static void SockadrToNetadr(const struct sockaddr_in *s, netadr_t *a)
{
	Com_Memset(a, 0, sizeof(*a));
	a->type = NA_IP;
	Com_Memcpy(a->ip, &s->sin_addr, sizeof(a->ip));
	a->port = s->sin_port;
}

/* ---------------------------------------------------------------------- */

static int NET_IPSocket(const char *net_interface, int port)
{
	int             newsocket;
	struct sockaddr_in address;
	int             i = 1;

	newsocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (newsocket < 0)
	{
		Com_Printf("WARNING: NET_IPSocket: socket: %s\n", strerror(errno));
		return -1;
	}

	/* Non-blocking so NET_Sleep's recvfrom() never stalls the frame loop. */
	if (setsockopt(newsocket, SOL_SOCKET, SO_NONBLOCK, (const char *)&i, sizeof(i)) < 0)
		Com_Printf("WARNING: NET_IPSocket: SO_NONBLOCK: %s\n", strerror(errno));

	/* Needed for LAN server discovery (NA_BROADCAST pings). */
	if (setsockopt(newsocket, SOL_SOCKET, SO_BROADCAST, (const char *)&i, sizeof(i)) < 0)
		Com_Printf("WARNING: NET_IPSocket: SO_BROADCAST: %s\n", strerror(errno));

	Com_Memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;

	if (!net_interface || !net_interface[0] || !Q_stricmp(net_interface, "localhost"))
		address.sin_addr.s_addr = INADDR_ANY;
	else
		address.sin_addr.s_addr = inet_addr(net_interface);

	if (address.sin_addr.s_addr == INADDR_NONE)
		address.sin_addr.s_addr = INADDR_ANY;

	address.sin_port = (port == PORT_ANY) ? 0 : BigShort((short)port);

	if (bind(newsocket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		Com_Printf("WARNING: NET_IPSocket: bind: %s\n", strerror(errno));
		close(newsocket);
		return -1;
	}

	{
		struct sockaddr_in bound;
		socklen_t blen = sizeof(bound);
		if (getsockname(newsocket, (struct sockaddr *)&bound, &blen) == 0)
		{
			const byte *bip = (const byte *)&bound.sin_addr;
			Com_Printf("NET_IPSocket: bound to %d.%d.%d.%d:%d\n",
			           bip[0], bip[1], bip[2], bip[3], (int)BigShort(bound.sin_port));
		}
	}

	return newsocket;
}

/* ---------------------------------------------------------------------- */

void NET_Config(qboolean enableNetworking)
{
	qboolean stop = !enableNetworking;
	qboolean start = enableNetworking && !networkingEnabled;

	if (!net_ip || !net_port)
		return;

	if (stop)
	{
		if (ip_socket >= 0)
		{
			close(ip_socket);
			ip_socket = -1;
		}
		networkingEnabled = qfalse;
		return;
	}

	if (start)
	{
		ip_socket = NET_IPSocket(net_ip->string, net_port->integer);
		if (ip_socket < 0)
		{
			Com_Printf("NET_Config: failed to open UDP socket on port %d, "
			           "networking disabled\n", net_port->integer);
			return;
		}
		networkingEnabled = qtrue;
		Com_Printf("NET_Config: UDP socket bound on port %d\n", net_port->integer);
	}
}

/* One-boot diagnostic: RCVBUF, read-length probe, send-to-self round trip. Logs to log.txt. */
static void NET_SelfTest(void)
{
	static const char probe[] = "\xff\xff\xff\xffwiiu_selftest";
	static byte        big[16384];
	struct sockaddr_in dst, from;
	socklen_t          flen;
	uint32_t           ip = 0;
	int                i, ret;
	int                rcvbuf = 0;
	socklen_t          optlen = sizeof(rcvbuf);
	static const int   lens[] = { 16384, 8192, 4096, 2048, 1400, 512 };

	if (ip_socket < 0)
		return;

	if (getsockopt(ip_socket, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen) == 0)
		Com_Printf("NET_SelfTest: default SO_RCVBUF = %d\n", rcvbuf);
	else
		Com_Printf("NET_SelfTest: getsockopt(SO_RCVBUF): errno=%d sockerr=%d\n",
		           errno, RPLWRAP(socketlasterr)());

	for (i = 0; i < (int)ARRAY_LEN(lens); i++)
	{
		errno = 0;
		flen = sizeof(from);
		ret = recvfrom(ip_socket, (char *)big, lens[i], 0,
		               (struct sockaddr *)&from, &flen);
		Com_Printf("NET_SelfTest: recvfrom(len=%d) ret=%d errno=%d sockerr=%d\n",
		           lens[i], ret, errno, RPLWRAP(socketlasterr)());
	}

	if (NNResult_IsFailure(ACGetAssignedAddress(&ip)) || !ip)
		return;

	Com_Memset(&dst, 0, sizeof(dst));
	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = ip; /* big-endian PPC: already network order */
	dst.sin_port = BigShort((short)net_port->integer);

	errno = 0;
	ret = sendto(ip_socket, probe, sizeof(probe) - 1, 0,
	             (struct sockaddr *)&dst, sizeof(dst));
	Com_Printf("NET_SelfTest: send-to-self ret=%d errno=%d sockerr=%d\n",
	           ret, errno, RPLWRAP(socketlasterr)());

	for (i = 0; i < 50; i++)
	{
		Sys_Sleep(10);
		errno = 0;
		flen = sizeof(from);
		ret = recvfrom(ip_socket, (char *)big, 1400, 0,
		               (struct sockaddr *)&from, &flen);
		if (ret >= 0)
		{
			Com_Printf("NET_SelfTest: SELF-RX OK, %d bytes after %d ms\n",
			           ret, (i + 1) * 10);
			return;
		}
	}
	Com_Printf("NET_SelfTest: SELF-RX FAILED: errno=%d sockerr=%d\n",
	           errno, RPLWRAP(socketlasterr)());
}

void NET_Init(void)
{
	ACConfigId configId;
	NNResult   result;
	uint32_t   assignedIp = 0;

	/* nn::ac must bring the connection up before sockets reach Wi-Fi. */
	result = ACInitialize();
	if (NNResult_IsFailure(result))
	{
		Com_Printf("NET_Init: ACInitialize failed, networking unavailable\n");
		return;
	}

	ACGetStartupId(&configId);
	result = ACConnect();
	if (NNResult_IsFailure(result))
	{
		Com_Printf("NET_Init: ACConnect failed, networking unavailable\n");
		ACFinalize();
		return;
	}

	ac_connected = qtrue;

	if (NNResult_IsSuccess(ACGetAssignedAddress(&assignedIp)) && assignedIp)
	{
		Com_Printf("NET_Init: nn::ac connected, assigned IP %u.%u.%u.%u\n",
		           (assignedIp >> 24) & 0xFF, (assignedIp >> 16) & 0xFF,
		           (assignedIp >> 8) & 0xFF, assignedIp & 0xFF);
	}
	else
	{
		Com_Printf("NET_Init: nn::ac connected but no IP assigned yet\n");
	}

	socket_lib_init();

	NET_DonateSocketMemory();

	net_ip   = Cvar_Get("net_ip", "0.0.0.0", CVAR_LATCH);
	net_port = Cvar_Get("net_port", va("%i", PORT_SERVER), CVAR_LATCH);

	Cmd_AddCommand("net_restart", NET_Restart_f);

	NET_Config(qtrue);

	NET_SelfTest();

	Com_Printf("NET_Init: UDP/IPv4 networking initialized on Wii U\n");
}

void NET_Shutdown(void)
{
	NET_Config(qfalse);

	/* Cancel somemopt donation wait BEFORE socket_lib_finish(), otherwise socket_lib_finish() deadlocks waiting on the detached donation thread. */
	Com_Printf("NET_Shutdown: cancelling somemopt donation wait...\n");
	{
		int r = somemopt(SOMEMOPT_REQUEST_CANCEL_WAIT, NULL, 0, SOMEMOPT_FLAGS_NONE);
		Com_Printf("NET_Shutdown: somemopt(CANCEL_WAIT) ret=%d errno=%d\n", r, errno);
	}

	socket_lib_finish();

	if (ac_connected)
	{
		ACClose();
		ACFinalize();
		ac_connected = qfalse;
	}
}

void NET_Restart_f(void)
{
	NET_Config(qfalse);
	NET_Config(qtrue);
}

void NET_JoinMulticast6(void)  { }
void NET_LeaveMulticast6(void) { }

/* ---------------------------------------------------------------------- */

/* Clamp to 1400 or IOSU fails EVERY recvfrom with EMSGSIZE, data or not. Two hw trips to find this. */
#define WIIU_MAX_UDP_READ 1400

static qboolean NET_GetPacket(netadr_t *net_from, msg_t *net_message)
{
	struct sockaddr_in from;
	socklen_t          fromlen;
	int                ret;
	int                maxread;

	if (ip_socket < 0)
		return qfalse;

	maxread = net_message->maxsize;
	if (maxread > WIIU_MAX_UDP_READ)
		maxread = WIIU_MAX_UDP_READ;

	fromlen = sizeof(from);
	ret = recvfrom(ip_socket, (char *)net_message->data, maxread,
	                0, (struct sockaddr *)&from, &fromlen);

	if (ret == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
			/* Loud (not DPrintf): inbound failures must show even at developer 0. */
			static int errCount;
			if (errCount < 5)
			{
				errCount++;
				Com_Printf("NET_GetPacket: errno=%d sockerr=%d\n",
				           errno, RPLWRAP(socketlasterr)());
			}
		}
		return qfalse;
	}

	SockadrToNetadr(&from, net_from);
	net_message->readcount = 0;
	net_message->cursize = ret;

	{
		static qboolean gotFirst;
		if (!gotFirst)
		{
			gotFirst = qtrue;
			Com_Printf("NET_GetPacket: first inbound packet: %d bytes from %s\n",
			           ret, NET_AdrToStringwPort(*net_from));
		}
	}
	return qtrue;
}

/* Drain cap so a packet flood can't spike a frame; excess packets defer to next call. */
#define WIIU_NET_EVENT_MAX_PACKETS 96

static void NET_Event(void)
{
	byte      bufData[MAX_MSGLEN + 1];
	netadr_t  from = { 0 };
	msg_t     netmsg;
	int       count = 0;
	static qboolean capWarned;

	while (1)
	{
		MSG_Init(&netmsg, bufData, sizeof(bufData));

		if (!NET_GetPacket(&from, &netmsg))
			break;

		if (com_sv_running->integer)
			Com_RunAndTimeServerPacket(&from, &netmsg);
		else
			CL_PacketEvent(from, &netmsg);

		if (++count >= WIIU_NET_EVENT_MAX_PACKETS)
		{
			if (!capWarned)
			{
				capWarned = qtrue;
				Com_Printf("NET_Event: drain cap hit (%d packets/frame) -- deferring rest\n",
				           WIIU_NET_EVENT_MAX_PACKETS);
			}
			break;
		}
	}
}

void NET_Sleep(int msec)
{
	if (msec < 0)  msec = 0;
	if (msec > 100) msec = 100;

	if (msec)
		Sys_Sleep(msec);

	/* No select() -- its fd-translation layer is unproven here, so drain recvfrom directly. */
	if (ip_socket >= 0)
		NET_Event();
}

/* ---------------------------------------------------------------------- */

qboolean NET_CompareBaseAdrMask(netadr_t a, netadr_t b, int netmask)
{
	int i, netmaskBytes;
	byte mask;

	if (a.type != b.type)
		return qfalse;

	if (a.type == NA_LOOPBACK)
		return qtrue;

	if (a.type != NA_IP)
		return NET_CompareBaseAdr(a, b);

	if (netmask < 0 || netmask > 32)
		netmask = 32;

	netmaskBytes = netmask / 8;
	for (i = 0; i < netmaskBytes; i++)
	{
		if (a.ip[i] != b.ip[i])
			return qfalse;
	}

	if (netmask % 8)
	{
		mask = (byte)(0xFF00 >> (netmask % 8));
		if ((a.ip[netmaskBytes] & mask) != (b.ip[netmaskBytes] & mask))
			return qfalse;
	}

	return qtrue;
}

qboolean NET_CompareBaseAdr(netadr_t a, netadr_t b)
{
	if (a.type != b.type)
		return qfalse;
	if (a.type == NA_LOOPBACK)
		return qtrue;
	if (a.type == NA_IP)
		return (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] &&
		        a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3]) ? qtrue : qfalse;
	return qfalse;
}

qboolean NET_CompareAdr(netadr_t a, netadr_t b)
{
	if (!NET_CompareBaseAdr(a, b))
		return qfalse;
	if (a.type == NA_IP)
		return (a.port == b.port) ? qtrue : qfalse;
	return qtrue;
}

qboolean NET_IsLocalAddress(netadr_t adr)
{
	return (adr.type == NA_LOOPBACK) ? qtrue : qfalse;
}

const char *NET_AdrToString(netadr_t a)
{
	static char s[64];
	if (a.type == NA_LOOPBACK)
		Com_sprintf(s, sizeof(s), "loopback");
	else if (a.type == NA_BOT)
		Com_sprintf(s, sizeof(s), "bot");
	else if (a.type == NA_BROADCAST)
		Com_sprintf(s, sizeof(s), "broadcast");
	else if (a.type == NA_IP)
		Com_sprintf(s, sizeof(s), "%i.%i.%i.%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
	else
		Com_sprintf(s, sizeof(s), "unknown");
	return s;
}

const char *NET_AdrToStringwPort(netadr_t a)
{
	static char s[64];
	if (a.type == NA_LOOPBACK)
		Com_sprintf(s, sizeof(s), "loopback");
	else if (a.type == NA_BOT)
		Com_sprintf(s, sizeof(s), "bot");
	else if (a.type == NA_BROADCAST)
		Com_sprintf(s, sizeof(s), "broadcast:%hu", BigShort(a.port));
	else
		Com_sprintf(s, sizeof(s), "%i.%i.%i.%i:%hu",
		            a.ip[0], a.ip[1], a.ip[2], a.ip[3], BigShort(a.port));
	return s;
}

/* Called by net_chan.c::NET_SendPacket for non-loopback destinations. */
void Sys_SendPacket(int length, const void *data, netadr_t to)
{
	struct sockaddr_in addr;

	/* IPv6 unsupported here -- drop silently, matching upstream's unopened ip6_socket. */
	if (to.type == NA_IP6 || to.type == NA_MULTICAST6)
		return;

	if (to.type != NA_IP && to.type != NA_BROADCAST)
	{
		Com_Printf("Sys_SendPacket: bad address type %d\n", to.type);
		return;
	}

	if (ip_socket < 0)
		return;

	NetadrToSockadr(&to, &addr);

	if (sendto(ip_socket, (const char *)data, length, 0,
	           (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		/* Loud (not DPrintf): a silent sendto failure already cost one debug session. */
		static int errCount;
		if (errCount < 5)
		{
			errCount++;
			Com_Printf("Sys_SendPacket: sendto failed: errno=%d sockerr=%d (to %s)\n",
			           errno, RPLWRAP(socketlasterr)(), NET_AdrToStringwPort(to));
		}
	}
	else
	{
		static qboolean sentFirst;
		if (!sentFirst)
		{
			sentFirst = qtrue;
			Com_Printf("Sys_SendPacket: first outbound packet accepted (%d bytes to %s)\n",
			           length, NET_AdrToStringwPort(to));
		}
	}
}

/* Resolves dotted-quad or hostnames (nsysnet DNS, needs AC connected). */
qboolean Sys_StringToAdr(const char *s, netadr_t *a, netadrtype_t family)
{
	struct in_addr  saddr;
	struct hostent *h;

	(void)family; /* IPv6 unsupported on this platform -- always NA_IP. */

	Com_Memset(a, 0, sizeof(*a));

	saddr.s_addr = inet_addr(s);
	if (saddr.s_addr != INADDR_NONE)
	{
		a->type = NA_IP;
		Com_Memcpy(a->ip, &saddr.s_addr, sizeof(a->ip));
		return qtrue;
	}

	h = gethostbyname(s);
	if (!h || !h->h_addr_list[0])
	{
		a->type = NA_BAD;
		return qfalse;
	}

	a->type = NA_IP;
	Com_Memcpy(a->ip, h->h_addr_list[0], sizeof(a->ip));
	return qtrue;
}

qboolean Sys_IsLANAddress(netadr_t adr)
{
	if (adr.type == NA_LOOPBACK)
		return qtrue;

	if (adr.type != NA_IP)
		return qfalse;

	/* RFC1918 + link-local, matches upstream net_ip.c. */
	if (adr.ip[0] == 10)
		return qtrue;
	if (adr.ip[0] == 172 && adr.ip[1] >= 16 && adr.ip[1] <= 31)
		return qtrue;
	if (adr.ip[0] == 192 && adr.ip[1] == 168)
		return qtrue;
	if (adr.ip[0] == 169 && adr.ip[1] == 254)
		return qtrue;
	if (adr.ip[0] == 127)
		return qtrue;

	return qfalse;
}

void Sys_ShowIP(void)
{
	if (ip_socket < 0 || !net_ip)
	{
		Com_Printf("IP: networking not initialized\n");
		return;
	}
	Com_Printf("IP: %s\n", net_ip->string);
}
