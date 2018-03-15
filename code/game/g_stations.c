/*
######################## ######################## ########################
	reload stations coded by #@(aka Raute)

	this are code for the padmod reload-stations(health/ammo)
	... there was never a ammo version ;P


missbrauchte entety-vars:
pos1[0] => timer for full-health-msg

pos2[0] => reloadtime (==0: kein reload geplant, !=0: reloadzeitpunkt)
pos2[1] => timer for min-touch-frequenz (max. 1touch/0.1sek)
pos2[2] => player touched this station (==0: kein touch seit think, ==1: player touched)

s.angles2[1] => loading-ring animation (0: no rings, 1: full ring animation)
s.angles2[2] => health in the station (0: leer, 1:voll ... wird in der cgame genutzt)


######################## ######################## ########################
*/

#include "g_local.h"

/*
	{"station_health", SP_station_health},
*/

static void Think_ReloadStation( gentity_t *ent )
{
	if(ent->pos2[2]==0.0f)//only move down if there is no player touch
	{
		if(ent->s.angles2[1]>0.01f)
			ent->s.angles2[1]-=0.0006f*(level.time - level.previousTime);//0.3 / 500 ;);
		else
			ent->s.angles2[1]=0.0f;
	}

/*(old: langsam aufladen)
	if(level.time-ent->pos2[0]>500)
	{
		if(ent->health<300)
		{
			ent->health+=(int)ent->speed;
			if(ent->health>300) ent->health=300;
		}

		ent->pos2[0]=(float)level.time;

		ent->s.apos.trDuration = ent->health;
		ent->s.angles2[2] = (float)ent->health/300.0f;
	}
*/
	if(ent->pos2[0] && level.time>ent->pos2[0])
	{
		gentity_t *te;

		ent->health=300;

		ent->s.apos.trDuration = ent->health;
		ent->s.angles2[2] = 1.0f;

		te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
		te->s.eventParm = G_SoundIndex( "sounds/healthstation/station_reloaded.wav" );
		te->r.svFlags |= SVF_BROADCAST;

		ent->pos2[0]=0.0f;
	}

	ent->nextthink = level.time+5;//think in the next frame
	ent->pos2[2]=0.0f;
}

static void Touch_ReloadStation( gentity_t *ent, gentity_t *other, trace_t *trace )
{
	if(!other->client)	return;

	if(ent->s.angles2[1]<0.99f)
		ent->s.angles2[1]+=0.0006f*(level.time - level.previousTime);//0.3 / 500 ;)
	else
		ent->s.angles2[1]=1.0f;

	ent->pos2[2]=1.0f;

	if(other->health>=100)
	{
		gentity_t *te;

		if(ent->pos1[0]+2000<level.time)
		{
			te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
			te->s.eventParm = G_SoundIndex( "sounds/healthstation/full_energy.wav" );
			te->r.svFlags |= SVF_BROADCAST;
			ent->pos1[0]=(float)level.time;
		}

		return;
	}

	if(ent->health<=0)
	{
		if(	ent->pos2[0]==0)
			ent->pos2[0]=(float)(level.time + 30000);

		return;
	}

	if(((float)level.time-ent->pos2[1])<100.0f)	return;//touch-event only every 0.1 sek

	other->health += ent->count;
	ent->health -= ent->count;
	if(other->health>100) other->health=100;
	other->client->ps.stats[STAT_HEALTH] = other->health;

	ent->pos2[1]=(float)level.time;

	ent->s.apos.trDuration = ent->health;
	ent->s.angles2[2] = (float)ent->health/300.0f;
}

/*
########################
SP_station_health
########################
*/
void SP_station_health( gentity_t *ent )
{
	ent->r.mins[0]=-40;
	ent->r.mins[1]=-40;
	ent->r.mins[2]=0;
	ent->r.maxs[0]=40;
	ent->r.maxs[1]=40;
	ent->r.maxs[2]=64;
	ent->r.contents = CONTENTS_TRIGGER;
	// ENTE sieht stations durch seinen port-tunnel
//	ent->r.svFlags |= SVF_BROADCAST; // vielleicht hilft das gegen den sound-bug *shrug*

	G_SetOrigin( ent, ent->s.origin );

	ent->think = Think_ReloadStation;
	ent->nextthink = level.time+5;
	ent->health = 300;//loaded stuff
	ent->count = 2;//1;//given to the player (0.1 sek)
	ent->speed = 1.0f;//wird nicht mehr benutz oder?

	ent->s.eType = ET_STATION;
	ent->touch = Touch_ReloadStation;
	ent->inuse = qtrue;
	trap_LinkEntity (ent);

	G_SoundIndex( "sounds/healthstation/full_energy.wav" );
	G_SoundIndex( "sounds/healthstation/station_reloaded.wav" );

	ent->s.apos.trDuration = ent->health;
	ent->s.angles2[2] = (float)ent->health/300.0f;
}
