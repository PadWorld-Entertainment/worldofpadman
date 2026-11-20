// make sure this is the same character as we use in chats in g_cmd.c
#define EC	"\x19"

//match template contexts
#define MTCONTEXT_MISC					2
#define MTCONTEXT_INITIALTEAMCHAT		4

//message types
#define MSG_NEWLEADER					1		//new leader
#define MSG_ENTERGAME					2		//enter game message
#define MSG_GOFORBALLOON				3		//accompany someone
#define MSG_STARTTEAMLEADERSHIP			4		//someone wants to become the team leader
#define MSG_WHOISTEAMLAEDER				5		//who is the team leader
#define MSG_GETITEM						6		//get an item
#define MSG_MAKELOVE					7		//
#define MSG_DROPCART					8		// cyr_drop
#define MSG_WRONGWALL					9		// cyr_spray
#define	MSG_CATCHME						10		// cyr_debug
#define MSG_FOLLOW 11
#define MSG_DEFEND 12
#define MSG_GETLOLLY 13
#define MSG_CAMP 14
#define MSG_ROAM 15
#define MSG_REPORT 16
#define MSG_STOPTEAMLEADERSHIP 17
#define MSG_DEFENDBALLOON 18
#define MSG_THAW 19
#define MSG_THAWME 20
#define MSG_WHATISMYCOMMAND 21


//command sub types
#define ST_ADDRESSED					1

//team message variables
#define NETNAME							0
#define PLACE							1
#define ADDRESSEE						2
#define ITEM							3
#define BALLOON							4


