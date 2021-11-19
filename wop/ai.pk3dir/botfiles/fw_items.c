// item fuzzy relations
#include "inv.h"

//initial powerup weights (overwrites individual values)
#define W_PADPOWER				1400	// overpowered, so go for it
#define W_CLIMBER				100		// not in the game yet
#define W_SPEEDY				700		// fucks up movement
#define W_JUMPER				900		// fucks up movement but looks good
#define W_VISIONLESS			1000
#define W_REVIVAL				1000
#define W_FLOATER				1000
#define W_KILLERDUCKS			1000
#define W_BAMBAM				2000
#define W_BOOMIE				2000

// weapons
#define W_SHOTGUN				1000
#define W_MACHINEGUN			400
#define W_GRENADELAUNCHER		1000
#define W_ROCKETLAUNCHER		1200
#define W_RAILGUN				900
#define W_BFG10K				1600
#define W_LIGHTNING				800
#define W_PLASMAGUN				1200

#define FAC_LUXUS1			0.3
#define FAC_LUXUS2			0.15

// SPRAY AMMO
// TODO: dont go for your own carts in ffa (neutral?)
// TODO: low weights for own carts in team-syc
weight "ammo_spray_b"{
	switch(INVENTORY_SPRAYPISTOLAMMO){
		case 1: return 1000;
		case 2: return 500;
		case 8: return 50;
		default: return 0;
	} //end switch
} //end weight
weight "ammo_spray_r"{
	switch(INVENTORY_SPRAYPISTOLAMMO){
		case 1: return 1000;
		case 2: return 500;
		case 8: return 5;
		default: return 0;
	} //end switch
} //end weight
weight "ammo_spray_n"{
	switch(INVENTORY_SPRAYPISTOLAMMO){
		case 1: return 1000;
		case 2: return 500;
		case 8: return 5;
		default: return 0;
	} //end switch
} //end weight

// ARMOR
weight "item_armor_shard"{
	switch(INVENTORY_ARMOR)	{
		case 1: return 15;
		case 200: return 5;
		default: return 0;
	} //end switch
} //end weight
weight "item_armor_padshield"{
	switch(INVENTORY_ARMOR)	{
		case 1: return 1000;
		case 200: return 30;
		default: return 0;
	} //end switch
} //end weight

//=============================================
// POWERUPS
//=============================================
weight "holdable_killerducks"{
	switch(INVENTORY_KILLERDUCKS){
		case 1:	{
			switch(INVENTORY_FLOATER){
				case 1: {
					switch(INVENTORY_BAMBAM){
						case 1:{
							switch(INVENTORY_BOOMIE){
								case 1: return W_KILLERDUCKS;
								default: return 0;
							}
						}
						default: return 0;
					}
				}
				default: return 0; //already has holdable
			} //end switch
		} //end case
		default: return 0; //already has a holdable
	} //end switch
} //end weight

weight "holdable_floater"{
	switch(INVENTORY_KILLERDUCKS){
		case 1:	{
			switch(INVENTORY_FLOATER){
				case 1: {
					switch(INVENTORY_BAMBAM){
						case 1:{
							switch(INVENTORY_BOOMIE){
								case 1: return W_FLOATER;
								default: return 0;
							}
						}
						default: return 0;
					}
				}
				default: return 0; //already has holdable
			} //end switch
		} //end case
		default: return 0; //already has a holdable
	} //end switch
} //end weight

weight "holdable_bambam"{
	switch(INVENTORY_KILLERDUCKS){
		case 1:	{
			switch(INVENTORY_FLOATER){
				case 1: {
					switch(INVENTORY_BAMBAM){
						case 1:{
							switch(INVENTORY_BOOMIE){
								case 1: return W_BAMBAM;
								default: return 0;
							}
						}
						default: return 0;
					}
				}
				default: return 0; //already has holdable
			} //end switch
		} //end case
		default: return 0; //already has a holdable
	} //end switch
} //end weight

weight "holdable_boomies"{
	switch(INVENTORY_KILLERDUCKS){
		case 1:	{
			switch(INVENTORY_FLOATER){
				case 1: {
					switch(INVENTORY_BAMBAM){
						case 1:{
							switch(INVENTORY_BOOMIE){
								case 1: return W_BOOMIE;
								default: return 0;
							}
						}
						default: return 0;
					}
				}
				default: return 0; //already has holdable
			} //end switch
		} //end case
		default: return 0; //already has a holdable
	} //end switch
} //end weight

weight "item_revival"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat(W_REVIVAL * 0.2);
		case 100:	return W_REVIVAL;
		default:	return W_REVIVAL;
	}
} //end weight
weight "item_visionless"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat( W_VISIONLESS * 0.2);
		case 100:	return W_VISIONLESS ;
		default:	return W_VISIONLESS ;
	}
} //end weight


weight "item_jumper"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat( W_JUMPER * 0.2);
		case 100:	return W_JUMPER ;
		default:	return W_JUMPER ;
	}
} //end weight


weight "item_speedy"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat( W_SPEEDY * 0.2);
		case 100:	return W_SPEEDY ;
		default:	return W_SPEEDY ;
	}
} //end weight


weight "item_climber"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat( W_CLIMBER * 0.2);
		case 100:	return W_CLIMBER ;
		default:	return W_CLIMBER ;
	}
} //end weight

weight "item_padpower"{
	switch(INV_GOTWEAPON){	// let unarmed dudes go for weaps first
		case 1:		return $evalfloat( W_PADPOWER * 0.2);
		case 100:	return W_PADPOWER ;
		default:	return W_PADPOWER ;
	}
} //end weight

//=============================================
// WEAPONS
//=============================================
weight "ammo_pumper"{
	switch( INVENTORY_PUMPER ){
		case 1: return 0;
		default:{
			switch(INVENTORY_PUMPERAMMO){
				case 1: return W_SHOTGUN;
				case 20: return 5;
				default: return 0;
			} // end switch
        }
	} //end switch
} //end weight

// no weapon at all? -> W_SHOTGUN
// some weapon but: no pumper or no pumper-ammo -> W_SHOTGUN scaled down by FAC_LUXUS1
// some more weapon: scale by FAC_LUXUS2 instead
weight "weapon_pumper"{

	switch(INV_GOTWEAPON){
		case 1:	return W_SHOTGUN;	// bot has no weap
		case 100:{		// bot is barely equiped with weapons
			switch(INVENTORY_PUMPER){
			
				case 1: return $evalfloat( W_SHOTGUN * FAC_LUXUS1 ) ;
				default:{
					switch( INVENTORY_PUMPERAMMO ){
						case 1:  return $evalfloat( W_SHOTGUN * FAC_LUXUS1 ) ;
						case 10: return 30;		// standardammo
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
		
		default:{	// bot is well equiped with weapons
			switch(INVENTORY_PUMPER){
				case 1: return $evalfloat( W_SHOTGUN * FAC_LUXUS2 ) ;
				default:{
					switch( INVENTORY_PUMPERAMMO ){
						case 1:  return $evalfloat( W_SHOTGUN * FAC_LUXUS2 ) ;
						case 10: return 10;		// standardammo
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
	} //end switch
} //end weight

weight "ammo_nipper"{
	switch(INVENTORY_NIPPER){
		case 1: return 0;
		default:{
			switch(INVENTORY_NIPPERAMMO){
				case 1:  return W_MACHINEGUN;
				case 50: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight

weight "weapon_nipper"{
	switch(INV_GOTWEAPON){
		case 1: return W_MACHINEGUN;
		case 100:{
			switch(INVENTORY_NIPPER){
				case 1: return $evalfloat(W_MACHINEGUN * FAC_LUXUS1) ;
				default:{
					switch(INVENTORY_NIPPERAMMO){
						case 1: return $evalfloat(W_MACHINEGUN * FAC_LUXUS1) ;
						case 50: return 30;
						case 90: return 5;
						default: return 0;
					}
				}//end case
			}// end switch
		}
		default:{
			switch(INVENTORY_NIPPER){
				case 1: return $evalfloat(W_MACHINEGUN * FAC_LUXUS2) ;
				default:{
					switch(INVENTORY_NIPPERAMMO){
						case 1: return $evalfloat(W_MACHINEGUN * FAC_LUXUS2) ;
						case 50: return 10;
						case 90: return 5;
						default: return 0;
					}
				}//end case
			}// end switch
		}
	}//end switch
} //end weight

weight "ammo_balloony"{
	switch(INVENTORY_BALLOONY){
		case 1: return 0;
		default:{
			switch(INVENTORY_BALLOONYAMMO){
				case 1:  return W_GRENADELAUNCHER;
				case 10: return 10;
				case 20: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight

weight "weapon_balloony"{
	switch(INV_GOTWEAPON){
		case 1: return W_GRENADELAUNCHER;
		case 100:{
			switch(INVENTORY_BALLOONY){
				case 1: return $evalfloat( W_GRENADELAUNCHER * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_BALLOONYAMMO){
						case 1: return $evalfloat( W_GRENADELAUNCHER * FAC_LUXUS1 );
						case 10: return 30;
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_BALLOONY){
				case 1: return $evalfloat( W_GRENADELAUNCHER * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_BALLOONYAMMO){
						case 1: return $evalfloat( W_GRENADELAUNCHER * FAC_LUXUS2 );
						case 10: return 10;
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "ammo_bubbleg"{
	switch(INVENTORY_BUBBLEG){
		case 1: return 0;
		default:{
			switch(INVENTORY_BUBBLEGAMMO){
				case 1:  return W_PLASMAGUN;
				case 50: return 10;
				case 70: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight
weight "weapon_bubbleg"{
	switch(INV_GOTWEAPON){
		case 1: return W_PLASMAGUN;
		case 100:{
			switch(INVENTORY_BUBBLEG){
				case 1: return $evalfloat( W_PLASMAGUN * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_BUBBLEGAMMO){
						case 1: return $evalfloat( W_PLASMAGUN * FAC_LUXUS1 );
						case 50: return 30;
						case 70: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_BUBBLEG){
				case 1: return $evalfloat( W_PLASMAGUN * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_BUBBLEGAMMO){
						case 1: return $evalfloat( W_PLASMAGUN * FAC_LUXUS2 );
						case 50: return 10;
						case 70: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "ammo_betty"{
	switch(INVENTORY_BETTY){
		case 1: return 0;
		default:{
			switch(INVENTORY_BETTYAMMO){
				case 1:  return W_ROCKETLAUNCHER;
				case 10: return 10;
				case 20: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight
weight "weapon_betty"{
	switch(INV_GOTWEAPON){
		case 1: return W_ROCKETLAUNCHER ;
		case 100:{
			switch(INVENTORY_BETTY){
				case 1: return $evalfloat( W_ROCKETLAUNCHER * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_BETTYAMMO){
						case 1: return $evalfloat( W_ROCKETLAUNCHER * FAC_LUXUS1 );
						case 10: return 30;
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_BETTY){
				case 1: return $evalfloat( W_ROCKETLAUNCHER * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_BETTYAMMO){
						case 1: return $evalfloat( W_ROCKETLAUNCHER * FAC_LUXUS2 );
						case 10: return 10;
						case 20: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "ammo_boaster"{
	switch(INVENTORY_BOASTER){
		case 1: return 0;
		default:{
			switch(INVENTORY_BOASTERAMMO){
				case 1:  return W_LIGHTNING;
				case 100: return 10;
				case 130: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight
weight "weapon_boaster"{
	switch(INV_GOTWEAPON){
		case 1: return W_LIGHTNING ;
		case 100:{
			switch(INVENTORY_BOASTER){
				case 1: return $evalfloat( W_LIGHTNING * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_BOASTERAMMO){
						case 1: return $evalfloat( W_LIGHTNING * FAC_LUXUS1 );
						case 100: return 30;
						case 130: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_BOASTER){
				case 1: return $evalfloat( W_LIGHTNING * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_BOASTERAMMO){
						case 1: return $evalfloat( W_LIGHTNING * FAC_LUXUS2 );
						case 100: return 10;
						case 130: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "ammo_splasher"{
	switch(INVENTORY_SPLASHER){
		case 1: return 0;
		default:{
			switch(INVENTORY_SPLASHERAMMO){
				case 1:  return W_RAILGUN;
				case 10: return 10;
				case 15: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight

weight "weapon_splasher"{
	switch(INV_GOTWEAPON){
		case 1: return W_RAILGUN ;
		case 100:{
			switch(INVENTORY_SPLASHER){
				case 1: return $evalfloat( W_RAILGUN * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_SPLASHERAMMO){
						case 1: return $evalfloat( W_RAILGUN * FAC_LUXUS1 );
						case 10: return 30;
						case 15: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_SPLASHER){
				case 1: return $evalfloat( W_RAILGUN * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_SPLASHERAMMO){
						case 1: return $evalfloat( W_RAILGUN * FAC_LUXUS2 );
						case 10: return 10;
						case 15: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "ammo_imperius"{
	switch(INVENTORY_IMPERIUS){
		case 1: return 0;
		default:{
			switch(INVENTORY_IMPERIUSAMMO){
				case 1:  return W_BFG10K;
				case 6: return W_BFG10K;	// need more ammo
				case 10: return 5;
				default: return 0;
			} //end switch
		}
	}
} //end weight

weight "weapon_imperius"{
	switch(INV_GOTWEAPON){
		case 1: return W_BFG10K ;
		case 100:{
			switch(INVENTORY_IMPERIUS){
				case 1: return $evalfloat( W_BFG10K * FAC_LUXUS1 );
				default:{
					switch(INVENTORY_IMPERIUSAMMO){
						case 1: return $evalfloat( W_BFG10K * FAC_LUXUS1 );
						case 10: return 30;
						case 15: return 5;
						default: return 0;
					}
				}
			}
		}
		default:{
			switch(INVENTORY_IMPERIUS){
				case 1: return $evalfloat( W_BFG10K * FAC_LUXUS2 );
				default:{
					switch(INVENTORY_IMPERIUSAMMO){
						case 1: return $evalfloat( W_BFG10K * FAC_LUXUS2 );
						case 10: return 10;
						case 15: return 5;
						default: return 0;
					}
				}
			}
		}
	}
} //end weight

weight "weapon_punchy"{	return 0; }
weight "weapon_spraypistol"{ return 0; }
weight "item_botroam"{	return 1; } //end weight

// flag carrier should show less interest in returning his own teams flag
weight "team_CTL_redlolly"{ 
/*	switch( INVENTORY_BLUEFLAG){
		case 1:{
			return 1000;
		}
		default:{
			return 200;
		}
	} 
*/
	return 4000;
}
weight "team_CTL_bluelolly"{
/*
	switch( INVENTORY_REDFLAG){
		case 1:{
			return 1000;
		}
		default:{
			return 200;
		}
	} 
*/
	return 4000;
}


