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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#ifndef G_SPAWN_H
#define G_SPAWN_H

void SP_info_player_start(gentity_t *ent);
void SP_info_player_deathmatch(gentity_t *ent);
void SP_info_player_intermission(gentity_t *ent);

void SP_func_plat(gentity_t *ent);
void SP_func_static(gentity_t *ent);
void SP_func_rotating(gentity_t *ent);
void SP_func_bobbing(gentity_t *ent);
void SP_func_pendulum(gentity_t *ent);
void SP_func_button(gentity_t *ent);
void SP_func_door(gentity_t *ent);
void SP_func_train(gentity_t *ent);
void SP_func_timer(gentity_t *self);
void SP_func_door_rotating(gentity_t *ent);

void SP_station_health(gentity_t *ent);
void SP_misc_sprayroomtl_teleporter(gentity_t *ent);
void SP_misc_externalmodel(gentity_t *ent);

void SP_trigger_always(gentity_t *ent);
void SP_trigger_multiple(gentity_t *ent);
void SP_trigger_push(gentity_t *ent);
void SP_trigger_teleport(gentity_t *ent);
void SP_trigger_hurt(gentity_t *ent);

void SP_trigger_balloonzone(gentity_t *ent);
void SP_trigger_exit(gentity_t *self);
void SP_trigger_forbiddenitems(gentity_t *self);

void SP_target_balloon(gentity_t *ent);

void SP_target_remove_powerups(gentity_t *ent);
void SP_target_give(gentity_t *ent);
void SP_target_delay(gentity_t *ent);
void SP_target_speaker(gentity_t *ent);
void SP_target_print(gentity_t *ent);
void SP_target_script(gentity_t *ent);
void SP_target_laser(gentity_t *self);
void SP_target_score(gentity_t *ent);
void SP_target_teleporter(gentity_t *ent);
void SP_target_relay(gentity_t *ent);
void SP_target_kill(gentity_t *ent);
void SP_target_position(gentity_t *ent);
void SP_target_location(gentity_t *ent);
void SP_target_push(gentity_t *ent);

void SP_light(gentity_t *self);
void SP_info_null(gentity_t *self);
void SP_info_notnull(gentity_t *self);
void SP_info_camp(gentity_t *self);
void SP_path_corner(gentity_t *self);

void SP_misc_teleporter_dest(gentity_t *self);
void SP_misc_model(gentity_t *ent);
void SP_misc_portal_camera(gentity_t *ent);
void SP_misc_portal_surface(gentity_t *ent);

void SP_shooter_rocket(gentity_t *ent);
void SP_shooter_plasma(gentity_t *ent);
void SP_shooter_grenade(gentity_t *ent);
void SP_shooter_killerduck(gentity_t *ent);

void SP_team_CTF_redplayer(gentity_t *ent);
void SP_team_CTF_blueplayer(gentity_t *ent);

void SP_team_CTF_redspawn(gentity_t *ent);
void SP_team_CTF_bluespawn(gentity_t *ent);

#endif
