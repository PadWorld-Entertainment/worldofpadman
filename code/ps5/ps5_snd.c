/*
===========================================================================
Quake3e for the PlayStation 5 - sound: Quake's DMA buffer played through the console's output.
Replaces code/sdl/sdl_snd.c.

Copyright (C) 1999-2005 Id Software, Inc. (the interfaces this implements)

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
===========================================================================
*/
#include <pthread.h>
#include <stdlib.h>

#include "../qcommon/q_shared.h"
#include "../client/snd_local.h"

#include "platform.h"

// The console's output is 48 kHz stereo 16-bit (s_khz 48, set at launch); its
// thread asks for 256 frames at a time and plays them out of the mixer's ring.
#define SND_SPEED 48000
#define SND_SAMPLES 16384 // mono samples in the ring: 8192 frames, about 170 ms

static qboolean snd_inited;
static int dmapos;
static int dmasize;
static pthread_mutex_t snd_lock = PTHREAD_MUTEX_INITIALIZER;


static void SNDDMA_Fill( int16_t *frames, int count, void *user )
{
	int bytes = count * 4;
	byte *out = (byte *)frames;

	pthread_mutex_lock( &snd_lock );
	if ( !snd_inited || !dma.buffer ) {
		memset( out, 0, bytes );
	} else {
		int pos = dmapos * 2; // dmapos counts mono 16-bit samples
		while ( bytes > 0 ) {
			int run = dmasize - pos;
			if ( run > bytes )
				run = bytes;
			memcpy( out, dma.buffer + pos, run );
			out += run;
			bytes -= run;
			pos = ( pos + run ) % dmasize;
		}
		dmapos = pos / 2;
	}
	pthread_mutex_unlock( &snd_lock );
}


qboolean SNDDMA_Init( void )
{
	if ( snd_inited )
		return qtrue;

	dmapos = 0;
	dma.samplebits = 16;
	dma.isfloat = 0;
	dma.channels = 2;
	dma.samples = SND_SAMPLES;
	dma.fullsamples = dma.samples / dma.channels;
	dma.submission_chunk = 1;
	dma.speed = SND_SPEED;
	dmasize = dma.samples * ( dma.samplebits / 8 );
	dma.buffer = calloc( 1, dmasize );
	if ( !dma.buffer )
		return qfalse;

	snd_inited = qtrue;
	if ( !audio_start( SNDDMA_Fill, NULL ) ) {
		Com_Printf( "PS5 sound: no output\n" );
		pthread_mutex_lock( &snd_lock );
		snd_inited = qfalse;
		pthread_mutex_unlock( &snd_lock );
		free( dma.buffer );
		dma.buffer = NULL;
		return qfalse;
	}
	Com_Printf( "PS5 sound: %d Hz, %d channels, %d bits\n", dma.speed, dma.channels, dma.samplebits );
	return qtrue;
}


int SNDDMA_GetDMAPos( void )
{
	return dmapos;
}


void SNDDMA_Shutdown( void )
{
	if ( !snd_inited )
		return;
	audio_stop();
	pthread_mutex_lock( &snd_lock );
	snd_inited = qfalse;
	free( dma.buffer );
	dma.buffer = NULL;
	dmapos = 0;
	pthread_mutex_unlock( &snd_lock );
}


void SNDDMA_Submit( void )
{
	pthread_mutex_unlock( &snd_lock );
}


void SNDDMA_BeginPainting( void )
{
	pthread_mutex_lock( &snd_lock );
}

void SNDDMA_SoundInfo( void ) { Com_Printf( "PS5 AudioOut: %d Hz, %d ch\n", dma.speed, dma.channels ); }
