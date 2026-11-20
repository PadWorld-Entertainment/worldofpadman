// weapon fuzzy relations
// A bot weapon file may #define these before including this file.
// Defaults keep the original curves.
#ifndef PUNCHY_CLOSE
#define PUNCHY_CLOSE			5
#endif
#ifndef PUNCHY_NEAR
#define PUNCHY_NEAR				3
#endif
#ifndef BUBBLEG_CLOSE
#define BUBBLEG_CLOSE			1
#endif
#ifndef BUBBLEG_FAR
#define BUBBLEG_FAR				1
#endif

weight "Punchy"
{
	switch(ENEMY_DIST)
	{
		case 1: return $evalint(W_GAUNTLET * PUNCHY_CLOSE);
		case 70: return $evalint(W_GAUNTLET * PUNCHY_NEAR);
		default: return $evalint(W_GAUNTLET*0.1);
	} //end switch
} //end weight

weight "Pumper"
{
	switch(INVENTORY_PUMPER)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_PUMPERAMMO)
			{
				case 1: return 0;
				default:
				{
					switch(ENEMY_DIST)
					{
						case 1: return $evalint( W_SHOTGUN * 0.1 );		// <---
						case 60: return W_SHOTGUN;
						case 1024: return W_SHOTGUN;
						case 1050: return $evalint(W_SHOTGUN*0.1);
						default: return 0;
					} //end switch
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Nipper"
{
	switch(INVENTORY_NIPPER)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_NIPPERAMMO)
			{
				case 1: return 0;
				case 10: return $evalint(W_MACHINEGUN*0.1);
				default:
				{
					return W_MACHINEGUN;		// <---
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Balloony"
{
	switch(INVENTORY_BALLOONY)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_BALLOONYAMMO)
			{
				case 1: return 0;
				default:
				{
					switch(ENEMY_DIST)
					{	// use for mid range
						case 1: return $evalint( W_GRENADELAUNCHER * 0.1 );		// <---
						case 200: return $evalint(W_GRENADELAUNCHER * 2 );
						case 800: return $evalint(W_GRENADELAUNCHER * 0.1 );
						default: return 1;
					} //end switch
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Betty"
{
	switch(INVENTORY_BETTY)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_BETTYAMMO)
			{
				case 1: return 0;
				default:
				{
					return W_ROCKETLAUNCHER;		// <---
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Boaster"
{
	switch(INVENTORY_BOASTER)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_BOASTERAMMO)
			{
				case 1: return 0;
				case 10: return $evalint(W_LIGHTNING*0.1);
				default:
				{

					//the lightning gun has a limited range
					switch(ENEMY_DIST)
					{
						case 1: return W_LIGHTNING;
						case 200: return $evalint(W_LIGHTNING*2);
						case 1024: return $evalint(W_LIGHTNING*0.1);
						default: return $evalint(W_LIGHTNING*0.1);		// <---
					} //end switch
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Splasher"
{
	switch(INVENTORY_SPLASHER)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_SPLASHERAMMO)
			{
				case 1: return 0;
				default:
				{
					return W_RAILGUN;		// <---
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "BubbleG"
{
	switch(INVENTORY_BUBBLEG)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_BUBBLEGAMMO)
			{
				case 1: return 0;
				case 2: return $evalint(W_PLASMAGUN*0.2);
				case 15: return W_PLASMAGUN;
				default:
				{
					// BUBBLEG_CLOSE/FAR default to 1, so this stays flat.
					switch(ENEMY_DIST)
					{
						case 1: return $evalint(W_PLASMAGUN * BUBBLEG_CLOSE);
						case 400: return W_PLASMAGUN;
						default: return $evalint(W_PLASMAGUN * BUBBLEG_FAR);
					} //end switch
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight

weight "Imperius"
{
	switch(INVENTORY_IMPERIUS)
	{
		case 1: return 0;
		default:
		{
			switch(INVENTORY_IMPERIUSAMMO)
			{
				case 1: return 0;
				default:
				{
					return W_BFG10K;		// <---
				} //end default
			} //end switch
		} //end default
	} //end switch
} //end weight
