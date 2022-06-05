// make sure this is the same character as we use in chats in g_cmd.c
#define EC "\x19"

// match template contexts
#define MTCONTEXT_MISC 2
#define MTCONTEXT_INITIALTEAMCHAT 4
#define MTCONTEXT_REPLYCHAT				128

// message types
#define MSG_NEWLEADER 1			  // new leader
#define MSG_ENTERGAME 2			  // enter game message
#define MSG_GOFORBALLOON 3		  // accompany someone
#define MSG_STARTTEAMLEADERSHIP 4 // someone wants to become the team leader
#define MSG_WHOISTEAMLAEDER 5	  // who is the team leader
#define MSG_GETITEM 6			  // get an item
#define MSG_MAKELOVE 7			  //
#define MSG_DROPCART 8			  // cyr_drop
#define MSG_WRONGWALL 9			  // cyr_spray
#define MSG_CATCHME 10			  // cyr_debug
#define MSG_CHATALL 200
#define MSG_CHATTEAM 201
#define MSG_CHATTELL 202

// command sub types
#define ST_ADDRESSED 1
#define ST_TEAM 32768

// team message variables
#define NETNAME 0
#define PLACE 1
#define MESSAGE 2
#define ADDRESSEE 2
#define ITEM 3
#define BALLOON 4
