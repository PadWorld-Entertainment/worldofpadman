#include "match.h"

#define TEAMCHAT EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ",

//entered the game message
MTCONTEXT_MISC
{
	//enter game message
	NETNAME, " entered the game" = (MSG_ENTERGAME, 0);
	//NETNAME, " is the new team leader" = (MSG_NEWLEADER, 0);
	NETNAME, "(Blue-Team) sprayed on the WRONG WALL" = (MSG_WRONGWALL, 0);
	NETNAME, "(Red-Team) sprayed on the WRONG WALL" =  (MSG_WRONGWALL, 0);
	NETNAME, " (BLUE NOSES Team) sprayed on the WRONG WALL!!!" = (MSG_WRONGWALL, 0);
	NETNAME, " (RED PADS Team) sprayed on the WRONG WALL!!!" = (MSG_WRONGWALL, 0);
} //end MTCONTEXT_ENTERGAME

//initial team command chat messages
MTCONTEXT_INITIALTEAMCHAT
{
	TEAMCHAT "I am the leader" = (MSG_STARTTEAMLEADERSHIP, 0);
	TEAMCHAT "I stop being the leader" = (MSG_STOPTEAMLEADERSHIP, 0);
	TEAMCHAT "follow me" = (MSG_FOLLOW, 0);
	TEAMCHAT ADDRESSEE, " follow me."|" follow me" = (MSG_FOLLOW, ST_ADDRESSED);
	TEAMCHAT "defend the base" = (MSG_DEFEND, 0);
	TEAMCHAT ADDRESSEE, " defend the base."|" defend the base" = (MSG_DEFEND, ST_ADDRESSED);
	TEAMCHAT "get the lolly"|"get enemy lolly" = (MSG_GETLOLLY, 0);
	TEAMCHAT ADDRESSEE, " get enemy lolly."|" get enemy lolly"|" get the lolly" = (MSG_GETLOLLY, ST_ADDRESSED);
	TEAMCHAT "camp here" = (MSG_CAMP, 0);
	TEAMCHAT ADDRESSEE, " camp here."|" camp here" = (MSG_CAMP, ST_ADDRESSED);
	TEAMCHAT "roam" = (MSG_ROAM, 0);
	TEAMCHAT ADDRESSEE, " roam."|" roam" = (MSG_ROAM, ST_ADDRESSED);
	TEAMCHAT "report"|"what are you doing?"|"what are you doing" = (MSG_REPORT, 0);
	TEAMCHAT ADDRESSEE, " report."|" report"|" what are you doing?" = (MSG_REPORT, ST_ADDRESSED);
	TEAMCHAT "who is the team leader?"|"who is the team leader" = (MSG_WHOISTEAMLAEDER, 0);
	TEAMCHAT "what is my command?"|"what is my command" = (MSG_WHATISMYCOMMAND, 0);
	TEAMCHAT "secure a balloon" = (MSG_GOFORBALLOON, 0);
	TEAMCHAT "defend a balloon"|"defend balloon" = (MSG_DEFENDBALLOON, 0);
	TEAMCHAT "thaw a teammate" = (MSG_THAW, 0);
	TEAMCHAT "thaw me!"|"thaw me" = (MSG_THAWME, 0);
	TEAMCHAT ADDRESSEE, " thaw me!"|" thaw me" = (MSG_THAWME, ST_ADDRESSED);
	TEAMCHAT ADDRESSEE, " give your cartridges to me."|" give your cartridges to me" = (MSG_DROPCART, ST_ADDRESSED);
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": give your cartridges to me" = (MSG_DROPCART,0);
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE," go secure ", BALLOON = (MSG_GOFORBALLOON, ST_ADDRESSED);

	// usefull for testing item reachability
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": collect ", ITEM = (MSG_GETITEM, 0);
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE, " collect ", ITEM = (MSG_GETITEM, ST_ADDRESSED);

	// usefull for debugging
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE, " catch me" = (MSG_CATCHME, ST_ADDRESSED);

} //end MTCONTEXT_INITIALTEAMCHAT

