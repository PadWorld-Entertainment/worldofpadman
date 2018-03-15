// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"

qboolean	G_SpawnString( const char *key, const char *defaultString, char **out ) {
	int		i;

	if ( !level.spawning ) {
		*out = (char *)defaultString;
//		G_Error( "G_SpawnString() called while not spawning" );
	}

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		if ( !Q_stricmp( key, level.spawnVars[i][0] ) ) {
			*out = level.spawnVars[i][1];
			return qtrue;
		}
	}

	*out = (char *)defaultString;
	return qfalse;
}

qboolean	G_SpawnFloat( const char *key, const char *defaultString, float *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atof( s );
	return present;
}

qboolean	G_SpawnInt( const char *key, const char *defaultString, int *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atoi( s );
	return present;
}

qboolean	G_SpawnVector( const char *key, const char *defaultString, float *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	sscanf( s, "%f %f %f", &out[0], &out[1], &out[2] );
	return present;
}



//
// fields are needed for spawning from the entity string
//
typedef enum {
	F_INT, 
	F_FLOAT,
	F_STRING,
	F_VECTOR,
	F_ANGLEHACK
} fieldtype_t;

typedef struct
{
	char	*name;
	int		ofs;
	fieldtype_t	type;
} field_t;

field_t fields[] = {
	{"classname", FOFS(classname), F_STRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"model", FOFS(model), F_STRING},
	{"model2", FOFS(model2), F_STRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"target", FOFS(target), F_STRING},
	{"targetname", FOFS(targetname), F_STRING},
	{"message", FOFS(message), F_STRING},
	{"team", FOFS(team), F_STRING},
	{"wait", FOFS(wait), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"dmg", FOFS(damage), F_INT},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},
	{"targetShaderName", FOFS(targetShaderName), F_STRING},
	{"targetShaderNewName", FOFS(targetShaderNewName), F_STRING},
	{"animationStart",FOFS(animationStart),F_INT},
	{"animationEnd",  FOFS(animationEnd),F_INT},
	{"animationFPS",  FOFS(animationFPS),F_FLOAT},
	{"distance", FOFS(distance), F_FLOAT}, 
	{NULL}
};


typedef struct {
	char	*name;
	void	(*spawn)(gentity_t *ent);
} spawn_t;

void SP_info_player_start (gentity_t *ent);
void SP_info_player_deathmatch (gentity_t *ent);
void SP_info_player_intermission (gentity_t *ent);

void SP_func_plat (gentity_t *ent);
void SP_func_static (gentity_t *ent);
void SP_func_rotating (gentity_t *ent);
void SP_func_bobbing (gentity_t *ent);
void SP_func_pendulum( gentity_t *ent );
void SP_func_button (gentity_t *ent);
void SP_func_door (gentity_t *ent);
void SP_func_train (gentity_t *ent);
void SP_func_timer (gentity_t *self);
void SP_func_door_rotating( gentity_t *ent );

void SP_station_health( gentity_t *ent );
void SP_misc_sprayroomtl_teleporter(gentity_t *ent);
void SP_misc_externalmodel(gentity_t *ent);

void SP_trigger_always (gentity_t *ent);
void SP_trigger_multiple (gentity_t *ent);
void SP_trigger_push (gentity_t *ent);
void SP_trigger_teleport (gentity_t *ent);
void SP_trigger_hurt (gentity_t *ent);

void SP_trigger_balloonzone (gentity_t *ent);
void SP_trigger_exit(gentity_t *self);
void SP_trigger_forbiddenitems( gentity_t *self );

void SP_target_balloon (gentity_t *ent);

void SP_target_remove_powerups( gentity_t *ent );
void SP_target_give (gentity_t *ent);
void SP_target_delay (gentity_t *ent);
void SP_target_speaker (gentity_t *ent);
void SP_target_print (gentity_t *ent);
void SP_target_script(gentity_t* ent);
void SP_target_laser (gentity_t *self);
void SP_target_score( gentity_t *ent );
void SP_target_teleporter( gentity_t *ent );
void SP_target_relay (gentity_t *ent);
void SP_target_kill (gentity_t *ent);
void SP_target_position (gentity_t *ent);
void SP_target_location (gentity_t *ent);
void SP_target_push (gentity_t *ent);

void SP_light (gentity_t *self);
void SP_info_null (gentity_t *self);
void SP_info_notnull (gentity_t *self);
void SP_info_camp (gentity_t *self);
void SP_path_corner (gentity_t *self);

void SP_misc_teleporter_dest (gentity_t *self);
void SP_misc_model(gentity_t *ent);
void SP_misc_portal_camera(gentity_t *ent);
void SP_misc_portal_surface(gentity_t *ent);

void SP_shooter_rocket( gentity_t *ent );
void SP_shooter_plasma( gentity_t *ent );
void SP_shooter_grenade( gentity_t *ent );
void SP_shooter_killerduck( gentity_t *ent );

void SP_team_CTF_redplayer( gentity_t *ent );
void SP_team_CTF_blueplayer( gentity_t *ent );

void SP_team_CTF_redspawn( gentity_t *ent );
void SP_team_CTF_bluespawn( gentity_t *ent );

void SP_item_botroam(gentity_t *ent)
{
//	int i;
}

spawn_t	spawns[] = {
	// info entities don't do anything at all, but provide positional
	// information for things controlled by other processes
	{"info_player_start", SP_info_player_start},
	{"info_player_deathmatch", SP_info_player_deathmatch},
	{"info_player_intermission", SP_info_player_intermission},
	{"info_null", SP_info_null},
	{"info_notnull", SP_info_notnull},		// use target_position instead
	{"info_camp", SP_info_camp},

	{"func_plat", SP_func_plat},
	{"func_button", SP_func_button},
	{"func_door", SP_func_door},
	{"func_static", SP_func_static},
	{"func_rotating", SP_func_rotating},
	{"func_bobbing", SP_func_bobbing},
	{"func_pendulum", SP_func_pendulum},
	{"func_train", SP_func_train},
	{"func_group", SP_info_null},
	{"func_timer", SP_func_timer},			// rename trigger_timer?
	{"func_door_rotating", SP_func_door_rotating},

	{"station_health", SP_station_health},

	{"misc_sprayroomtl_teleporter", SP_misc_sprayroomtl_teleporter},

	// Triggers are brush objects that cause an effect when contacted
	// by a living player, usually involving firing targets.
	// While almost everything could be done with
	// a single trigger class and different targets, triggered effects
	// could not be client side predicted (push and teleport).
	{"trigger_always", SP_trigger_always},
	{"trigger_multiple", SP_trigger_multiple},
	{"trigger_push", SP_trigger_push},
	{"trigger_teleport", SP_trigger_teleport},
	{"trigger_hurt", SP_trigger_hurt},

	{"trigger_balloonzone", SP_trigger_balloonzone},
	{"trigger_forbiddenitems", SP_trigger_forbiddenitems},
	{"trigger_exit",SP_trigger_exit},

	// targets perform no action by themselves, but must be triggered
	// by another entity
	{"target_balloon", SP_target_balloon}, 

	{"target_give", SP_target_give},
	{"target_remove_powerups", SP_target_remove_powerups},
	{"target_delay", SP_target_delay},
	{"target_speaker", SP_target_speaker},
	{"target_print", SP_target_print},
	{"target_script", SP_target_script},
	{"target_laser", SP_target_laser},
	{"target_score", SP_target_score},
	{"target_teleporter", SP_target_teleporter},
	{"target_relay", SP_target_relay},
	{"target_kill", SP_target_kill},
	{"target_position", SP_target_position},
	{"target_location", SP_target_location},
	{"target_push", SP_target_push},

	{"light", SP_light},
	{"path_corner", SP_path_corner},

	{"misc_teleporter_dest", SP_misc_teleporter_dest},
	{"misc_model", SP_misc_model},

	{"misc_externalmodel",SP_misc_externalmodel},

	{"misc_portal_surface", SP_misc_portal_surface},
	{"misc_portal_camera", SP_misc_portal_camera},

	{"shooter_betty", SP_shooter_rocket},
	{"shooter_balloony", SP_shooter_grenade},
	{"shooter_bubbleg", SP_shooter_plasma},
	{"shooter_killerduck", SP_shooter_killerduck},

	{"team_redplayer", SP_team_CTF_redplayer},
	{"team_blueplayer", SP_team_CTF_blueplayer},

	{"team_redspawn", SP_team_CTF_redspawn},
	{"team_bluespawn", SP_team_CTF_bluespawn},

	{"item_botroam", SP_item_botroam},
	{0, 0}
};

/*
===============
G_CallSpawn

Finds the spawn function for the entity and calls it,
returning qfalse if not found
===============
*/
qboolean G_CallSpawn( gentity_t *ent ) {
	spawn_t	*s;
	gitem_t	*item;

	if ( !ent->classname ) {
		G_Printf ("G_CallSpawn: NULL classname\n");
		return qfalse;
	}

	// check item spawn functions
	for ( item=bg_itemlist+1 ; item->classname ; item++ ) {
		if ( !strcmp(item->classname, ent->classname) ) {
			G_SpawnItem( ent, item );
			return qtrue;
		}
	}

	// check normal spawn functions
	for ( s=spawns ; s->name ; s++ ) {
		if ( !strcmp(s->name, ent->classname) ) {
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}
	G_Printf ("%s doesn't have a spawn function\n", ent->classname);
	return qfalse;
}

/*
=============
G_NewString

Builds a copy of the string, translating \n to real linefeeds
so message texts can be multi-line
=============
*/
char *G_NewString( const char *string ) {
	char	*newb, *new_p;
	int		i,l;
	
	l = strlen(string) + 1;

	newb = G_Alloc( l );

	new_p = newb;

	// turn \n into a real linefeed
	for ( i=0 ; i< l ; i++ ) {
		if (string[i] == '\\' && i < l-1) {
			i++;
			if (string[i] == 'n') {
				*new_p++ = '\n';
			} else {
				*new_p++ = '\\';
			}
		} else {
			*new_p++ = string[i];
		}
	}
	
	return newb;
}




/*
===============
G_ParseField

Takes a key/value pair and sets the binary values
in a gentity
===============
*/
void G_ParseField( const char *key, const char *value, gentity_t *ent ) {
	field_t	*f;
	byte	*b;
	float	v;
	vec3_t	vec;

	for ( f=fields ; f->name ; f++ ) {
		if ( !Q_stricmp(f->name, key) ) {
			// found it
			b = (byte *)ent;

			switch( f->type ) {
			case F_STRING:
				*(char **)(b+f->ofs) = G_NewString (value);
				break;
			case F_VECTOR:
				sscanf (value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
				((float *)(b+f->ofs))[0] = vec[0];
				((float *)(b+f->ofs))[1] = vec[1];
				((float *)(b+f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int *)(b+f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(float *)(b+f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((float *)(b+f->ofs))[0] = 0;
				((float *)(b+f->ofs))[1] = v;
				((float *)(b+f->ofs))[2] = 0;
				break;
			}
			return;
		}
	}
}


typedef struct {
	const char* s; //search
	const char* r; //replace
} replacePair_t;

replacePair_t q3ToWopItems[] = {
	{ "weapon_gauntlet",		"weapon_punchy"		},
	{ "weapon_machinegun",		"weapon_nipper"		},
	{ "weapon_shotgun",			"weapon_pumper"		},
	{ "weapon_lightning",		"weapon_boaster"	},
	{ "weapon_railgun",			"weapon_splasher"	},
	{ "weapon_plasmagun",		"weapon_bubbleg"	},
	{ "weapon_grenadelauncher",	"weapon_balloony"	},
	{ "weapon_rocketlauncher",	"weapon_betty"		},
	{ "weapon_bfg",				"weapon_imperius"	},
			
	{ "ammo_shells",	"ammo_pumper"	},
	{ "ammo_bullets",	"ammo_nipper"	},
	{ "ammo_grenades",	"ammo_balloony"	},
	{ "ammo_cells",		"ammo_bubbleg"	},
	{ "ammo_lightning",	"ammo_boaster"	},
	{ "ammo_rockets",	"ammo_betty"	},
	{ "ammo_slugs",		"ammo_splasher"	},
	{ "ammo_bfg",		"ammo_imperius"	},
			
	{ "item_quad",		"item_padpower"			},
	{ "item_enviro",	"item_climber"			},
	{ "item_hast",		"item_speedy"			},
	{ "item_flight",	"item_jump"				},
	{ "item_invis",		"item_visionless"		},
	{ "item_regen",		"item_revival"			},
	{ "item_armor_body","item_armor_padshield"	},
			
	{ "team_CTF_redflag",	"team_CTL_redlolly"	},
	{ "team_CTF_blueflag",	"team_CTL_bluelolly"},
	{ "team_CTF_redplayer",	"team_redplayer"	},
	{ "team_CTF_blueplayer","team_blueplayer"	},
	{ "team_CTF_redspawn",	"team_redspawn"		},
	{ "team_CTF_bluespawn",	"team_bluespawn"	},
	{ NULL,		NULL }
};

replacePair_t shortMarkernames[] = {
	{ "black",	"models/mapobjects/pad_weaponmarker/pad_wepm_black_bg.md3"	},
	{ "blue",	"models/mapobjects/pad_weaponmarker/pad_wepm_blue_bg.md3"	},
	{ "green",	"models/mapobjects/pad_weaponmarker/pad_wepm_green_gg.md3"	},
	{ "purple",	"models/mapobjects/pad_weaponmarker/pad_wepm_green_bg.md3"	},
	{ "orange",	"models/mapobjects/pad_weaponmarker/pad_wepm_orange_bg.md3"	},
	{ NULL,		NULL }
};

replacePair_t spawnpointReplacements[] = {
	{ "team_redplayer",		"info_player_deathmatch" },
	{ "team_blueplayer",	"info_player_deathmatch" },
	{ "team_redspawn",		"info_player_deathmatch" },
	{ "team_bluespawn",		"info_player_deathmatch" },
	{ NULL,					NULL }
};

/*
===================
G_SpawnGEntityFromSpawnVars

Spawn an entity and fill in all of the level fields from
level.spawnVars[], then call the class specfic spawn function
===================
*/
void G_SpawnGEntityFromSpawnVars( void ) {
	int			i;
	gentity_t	*ent;
	char		*s, *value;
	const char	*gametypeName;
	gitem_t		*item;

	static const char *gametypeNames[] = {"ffa", "tournament", "single", "spray", "lps", "team", "ctl", "sptp", "balloon"};

	// get the next free entity
	ent = G_Spawn();

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], ent );
	}

	// Convert suitable items from q3
	if ( g_q3Items.integer ) {
		for ( i = 0; q3ToWopItems[i].s; i++ ) {
			if ( Q_stricmp( ent->classname, q3ToWopItems[i].s ) == 0 ) {
				ent->classname = (char*)q3ToWopItems[i].r;
				break;
			}
		}
	}

	// Convert additional team spawnpoints when not in team gametypes
	if ( g_gametype.integer < GT_TEAM ) {
		for ( i = 0; spawnpointReplacements[i].s; i++ ) {
			if ( Q_stricmp( ent->classname, spawnpointReplacements[i].s ) == 0 ) {
				ent->classname = (char*)spawnpointReplacements[i].r;
				break;
			}
		}
	}

	// remove lollies if we aren't in CTL
	// FIXME: Should mappers do this via gametype spawnvar (also for balloons, lps items etc) ?
	if( g_gametype.integer!=GT_CTF
		&& (!Q_stricmp(ent->classname,"team_CTL_redlolly") || !Q_stricmp(ent->classname,"team_CTL_bluelolly")) )
	{
		G_FreeEntity( ent );
		return;
	}


	if ( !IsSyc() ) {
		G_SpawnInt( "onlyspraygt", "0", &i );
		if ( i ) {
			G_FreeEntity( ent );
			return;
		}
	}

	if( g_gametype.integer == GT_LPS ) {
		if( !Q_stricmp(ent->classname,"station_health")
				|| NULL!=strstr(ent->classname,"_imperius")
				|| !Q_stricmpn(ent->classname,"holdable_",9)
				|| (!Q_stricmpn(ent->classname,"item_",5)
					&& Q_stricmp(ent->classname,"item_botroam")) ) {
			G_FreeEntity( ent );
			return;
		}

		G_SpawnInt( "notLPS", "0", &i );
		if( i ) {
			G_FreeEntity( ent );
			return;
		}
	}

	// check for "notsingle" flag
	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		G_SpawnInt( "notsingle", "0", &i );
		if ( i ) {
			G_FreeEntity( ent );
			return;
		}
	}
	// check for "notteam" flag (GT_FFA, GT_TOURNAMENT, GT_SINGLE_PLAYER)
	if ( g_gametype.integer >= GT_TEAM ) {
		G_SpawnInt( "notteam", "0", &i );
		if ( i ) {
			G_FreeEntity( ent );
			return;
		}
	} else {
		G_SpawnInt( "notfree", "0", &i );
		if ( i ) {
			G_FreeEntity( ent );
			return;
		}
	}

	G_SpawnInt( "notwop", "0", &i );
	if ( i ) {
		G_FreeEntity( ent );
		return;
	}

	if( G_SpawnString( "gametype", NULL, &value ) ) {
		if( g_gametype.integer >= GT_FFA && g_gametype.integer < GT_MAX_GAME_TYPE ) {
			gametypeName = gametypeNames[g_gametype.integer];

			s = strstr( value, gametypeName );
			if( !s ) {
				G_FreeEntity( ent );
				return;
			}
		}
	}

	if( G_SpawnString( "notGametype", NULL, &value ) ) {
		if( g_gametype.integer >= GT_FFA && g_gametype.integer < GT_MAX_GAME_TYPE ) {
			gametypeName = gametypeNames[g_gametype.integer];

			s = strstr( value, gametypeName );
			if( s ) {
				G_FreeEntity( ent );
				return;
			}
		}
	}


	for ( item = ( bg_itemlist + 1 ); item->classname; item++ ) {
    	if ( strcmp( item->classname, ent->classname ) == 0 ) {
			RegisterItem( item );
			break;
		}
	}


	/*
	Modifiers / Instagib
	Exclude unwanted Items from spawning in Instagib, but have them register with
	the client anyway. (So we have working visuals and sound for the entities when
	we turn off the modifier, which doesn't trigger a full client reload)
	*/
	if ( g_modInstagib.integer && !Instagib_canSpawnEntity( ent ) ) {
		G_FreeEntity( ent );
		return;
	}


	// weapon marker
	if( G_SpawnString("marker", NULL, &value) ) {
		trace_t		tr;
		vec3_t		min = {-16.0f,-16.0f, 0.0f};
		vec3_t		max = { 16.0f, 16.0f, 1.0f};
		vec3_t		trEnd;

		if ( g_modInstagib.integer ) { // no markers for instagib
			G_FreeEntity( ent );
			return;
		}

		VectorCopy( ent->s.origin, trEnd );
		trEnd[2] -= 512;
		trap_Trace(&tr,ent->s.origin,min,max,trEnd,ENTITYNUM_NONE,MASK_SHOT);

		if(!tr.startsolid && tr.fraction!=1.0f) {
			gentity_t	*marker_ent;
			marker_ent = G_Spawn();

			marker_ent->classname = "misc_externalmodel";
			for(i=0;shortMarkernames[i].s!=NULL;++i) {
				if(!Q_stricmp(value,shortMarkernames[i].s))
					marker_ent->model = (char *)shortMarkernames[i].r;
			}
			if(marker_ent->model == NULL)
				marker_ent->model = G_NewString(value);
			marker_ent->s.modelindex = G_ModelIndex( marker_ent->model );

			VectorCopy( ent->s.angles, marker_ent->s.angles );

			G_SetOrigin( marker_ent, tr.endpos );

			VectorCopy( marker_ent->s.angles, marker_ent->s.apos.trBase );

			trap_LinkEntity(marker_ent);
		}
	}

	// move editor origin to pos
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );

	// if we didn't get a classname, don't bother spawning anything
	if ( !G_CallSpawn( ent ) ) {
		G_FreeEntity( ent );
	}
}



/*
====================
G_AddSpawnVarToken
====================
*/
char *G_AddSpawnVarToken( const char *string ) {
	int		l;
	char	*dest;

	l = strlen( string );
	if ( level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS ) {
		G_Error( "G_AddSpawnVarToken: MAX_SPAWN_CHARS" );
	}

	dest = level.spawnVarChars + level.numSpawnVarChars;
	memcpy( dest, string, l+1 );

	level.numSpawnVarChars += l + 1;

	return dest;
}

/*
====================
G_ParseSpawnVars

Parses a brace bounded set of key / value pairs out of the
level's entity strings into level.spawnVars[]

This does not actually spawn an entity.
====================
*/
qboolean G_ParseSpawnVars( void ) {
	char		keyname[MAX_TOKEN_CHARS];
	char		com_token[MAX_TOKEN_CHARS];

	level.numSpawnVars = 0;
	level.numSpawnVarChars = 0;

	// parse the opening brace
	if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
		// end of spawn string
		return qfalse;
	}
	if ( com_token[0] != '{' ) {
		G_Error( "G_ParseSpawnVars: found %s when expecting {",com_token );
	}

	// go through all the key / value pairs
	while ( 1 ) {	
		// parse key
		if ( !trap_GetEntityToken( keyname, sizeof( keyname ) ) ) {
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( keyname[0] == '}' ) {
			break;
		}
		
		// parse value	
		if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( com_token[0] == '}' ) {
			G_Error( "G_ParseSpawnVars: closing brace without data" );
		}
		if ( level.numSpawnVars == MAX_SPAWN_VARS ) {
			G_Error( "G_ParseSpawnVars: MAX_SPAWN_VARS" );
		}
		level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( keyname );
		level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( com_token );
		level.numSpawnVars++;
	}

	return qtrue;
}



/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"		music wav file
"gravity"	800 is default gravity
"message"	Text to print during connection process
*/
void SP_worldspawn( void ) {
	char	*s;

	G_SpawnString( "classname", "", &s );
	if ( Q_stricmp( s, "worldspawn" ) ) {
		G_Error( "SP_worldspawn: The first entity isn't 'worldspawn'" );
	}

	// make some data visible to connecting client
	trap_SetConfigstring( CS_GAME_VERSION, GAME_VERSION );

	trap_SetConfigstring( CS_LEVEL_START_TIME, va("%i", level.startTime ) );

	G_SpawnString( "music", "", &s );
	trap_SetConfigstring( CS_MUSIC, s );

	G_SpawnString( "message", "", &s );
	trap_SetConfigstring( CS_MESSAGE, s );				// map specific message

	trap_SetConfigstring( CS_MOTD, g_motd.string );		// message of the day

	G_SpawnString( "gravity", "800", &s );
	trap_Cvar_Set( "g_gravity", s );

	G_SpawnString( "enableDust", "0", &s );
	trap_Cvar_Set( "g_enableDust", s );

	G_SpawnString( "enableBreath", "0", &s );
	trap_Cvar_Set( "g_enableBreath", s );

	G_SpawnInt("maxsprayroomtime","30",&level.maxsprayroomtime);
	{
		char	tmpstr[256];
		vec3_t	tmpv3;

		G_SpawnString("skylensflare_dir","10 10 10", &s);
		sscanf(s,"%f %f %f",&tmpv3[0],&tmpv3[1],&tmpv3[2]);
		VectorNormalize(tmpv3);
		
		G_SpawnString("skylensflare","", &s);
		Com_sprintf(tmpstr, 256, "%1.3f %1.3f %1.3f >%.128s",tmpv3[0],tmpv3[1],tmpv3[2],s);

		trap_Cvar_Set( "g_skyLensflare", tmpstr );
	}

	G_SpawnString( "wopSky", "", &s );
	trap_Cvar_Set( "g_sky", s );

	g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
	g_entities[ENTITYNUM_WORLD].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_WORLD].classname = "worldspawn";

	g_entities[ENTITYNUM_NONE].s.number = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].classname = "nothing";

	// see if we want a warmup time
	trap_SetConfigstring( CS_WARMUP, "" );
	if ( g_restarted.integer ) {
		trap_Cvar_Set( "g_restarted", "0" );
		level.warmupTime = 0;
	}
	else if ( g_doWarmup.integer ) { // Turn it on
		level.warmupTime = -1;
		trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
		G_LogPrintf( "Warmup:\n" );
	}
	else if(g_gametype.integer==GT_LPS) {
		if(g_warmup.integer>=20)
			level.warmupTime = g_warmup.integer;
		else
		{
			level.warmupTime = 20;
			trap_Cvar_Set("g_warmup","20");
		}
		trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
		G_LogPrintf( "Warmup:\n" );
	}

}


/*
==============
G_SpawnEntitiesFromString

Parses textual entity definitions out of an entstring and spawns gentities.
==============
*/
void G_SpawnEntitiesFromString( void ) {
	// allow calls to G_Spawn*()
	level.spawning = qtrue;
	level.numSpawnVars = 0;
	level.sr_tl_tele = NULL;

	// the worldspawn is not an actual entity, but it still
	// has a "spawn" function to perform any global setup
	// needed by a level (setting configstrings or cvars, etc)
	if ( !G_ParseSpawnVars() ) {
		G_Error( "SpawnEntities: no entities" );
	}
	SP_worldspawn();

	// parse ents
	while( G_ParseSpawnVars() ) {
		G_SpawnGEntityFromSpawnVars();
	}	

	level.spawning = qfalse;			// any future calls to G_Spawn*() will be errors
}

