// weapon fuzzy relations

weight "Punchy"
{
	switch(ENEMY_DIST)
	{
		case 1: return $evalint(W_GAUNTLET *5);
		case 70: return $evalint(W_GAUNTLET *3);
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
					return W_PLASMAGUN;		// <---
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
