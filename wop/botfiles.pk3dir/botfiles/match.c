#include "match.h"

//entered the game message
MTCONTEXT_MISC
{
	//enter game message
	NETNAME, " entered the game" = (MSG_ENTERGAME, 0);
	//NETNAME, " is the new team leader" = (MSG_NEWLEADER, 0);
	NETNAME, "(Blue-Team) sprayed on the WRONG WALL" = (MSG_WRONGWALL, 0);
	NETNAME, "(Red-Team) sprayed on the WRONG WALL" =  (MSG_WRONGWALL, 0);
} //end MTCONTEXT_ENTERGAME

//initial team command chat messages
MTCONTEXT_INITIALTEAMCHAT
{
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": give your cartridges to me" = (MSG_DROPCART,0);
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE," go secure ", BALLOON = (MSG_GOFORBALLOON, ST_ADDRESSED);

	// usefull for testing item reachability
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": collect ", ITEM = (MSG_GETITEM, 0);
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE, " collect ", ITEM = (MSG_GETITEM, ST_ADDRESSED);
	
	// usefull for debugging
	EC"("|EC"[", NETNAME, EC")"|EC"]", PLACE, EC": ", ADDRESSEE, " catch me" = (MSG_CATCHME, ST_ADDRESSED);

} //end MTCONTEXT_INITIALTEAMCHAT

