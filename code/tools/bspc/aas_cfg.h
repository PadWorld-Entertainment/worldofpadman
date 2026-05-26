/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#define BBOXFL_GROUNDED 1	 // bounding box only valid when on ground
#define BBOXFL_NOTGROUNDED 2 // bounding box only valid when NOT on ground

typedef struct cfg_s {
	int numbboxes;					   // Number of configured bounding boxes.
	aas_bbox_t bboxes[AAS_MAX_BBOXES]; // Configured bounding boxes.
	int allpresencetypes;			   // Bitwise OR of all configured bbox presence types.
	// aas settings
	vec3_t phys_gravitydirection;  // Normalized gravity direction vector.
	float phys_friction;		   // Friction applied while moving on ground.
	float phys_stopspeed;		   // Minimum speed used when applying friction.
	float phys_gravity;			   // Gravity acceleration outside water.
	float phys_waterfriction;	   // Friction applied while swimming.
	float phys_watergravity;	   // Gravity acceleration while swimming.
	float phys_maxvelocity;		   // Velocity cap used when solving jump reachability.
	float phys_maxwalkvelocity;	   // Maximum walking speed.
	float phys_maxcrouchvelocity;  // Maximum crouched walking speed.
	float phys_maxswimvelocity;	   // Maximum swimming speed.
	float phys_walkaccelerate;	   // Acceleration applied while walking on ground.
	float phys_airaccelerate;	   // Acceleration applied while airborne.
	float phys_swimaccelerate;	   // Acceleration applied while swimming.
	float phys_maxstep;			   // Maximum step height treated as walkable.
	float phys_maxsteepness;	   // Ground-slope threshold used to classify faces as walkable.
	float phys_maxwaterjump;	   // Maximum vertical distance for water-jump reachability.
	float phys_maxbarrier;		   // Maximum obstacle height for barrier-jump reachability.
	float phys_jumpvel;			   // Initial upward jump velocity.
	float phys_falldelta5;		   // Fall-damage delta threshold treated as 5 damage.
	float phys_falldelta10;		   // Fall-damage delta threshold treated as 10 damage.
	float rs_waterjump;			   // Travel-time cost for water-jump reachability.
	float rs_teleport;			   // Travel-time cost for teleporter reachability.
	float rs_barrierjump;		   // Travel-time cost for barrier-jump reachability.
	float rs_startcrouch;		   // Extra travel-time cost for entering a crouch-only area.
	float rs_startgrapple;		   // Base travel-time cost for grapple reachability.
	float rs_startwalkoffledge;	   // Base travel-time cost for walk-off-ledge reachability.
	float rs_startjump;			   // Base travel-time cost for jump reachability.
	float rs_rocketjump;		   // Travel-time cost for rocket-jump reachability.
	float rs_bfgjump;			   // Travel-time cost for BFG-jump reachability.
	float rs_jumppad;			   // Travel-time cost for jumppad reachability.
	float rs_aircontrolledjumppad; // Travel-time cost for air-controlled jumppad reachability.
	float rs_funcbob;			   // Travel-time cost for func_bobbing reachability.
	float rs_startelevator;		   // Base travel-time cost for elevator reachability.
	float rs_falldamage5;		   // Extra travel-time cost for falls above phys_falldelta5.
	float rs_falldamage10;		   // Extra travel-time cost for falls above phys_falldelta10.
	float rs_maxfallheight;		// Maximum drop height for generated walk-off-ledge reachability; 0 disables the limit.
	float rs_maxjumpfallheight; // Maximum fall height assumed when estimating jump reachability distance.
} cfg_t;

extern cfg_t cfg;

void DefaultCfg(void);
int LoadCfgFile(const char *filename);
