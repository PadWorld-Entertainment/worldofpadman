/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/wiiu/wiiu_pak_extract.c -- self-extracting bundled pak (per flavor).
 *
 * The flavor's own pak (our content, not retail data) rides baked into the
 * .rpx as a C byte array and is unpacked to SD on first boot (length+checksum
 * gated) so nobody has to copy it there by hand:
 *
 *   q3       fixes/baseq3/pak9-wiiu.pk3       -> sd:/quake3/baseq3/
 *   ta       fixes/missionpack/pak4-wiiu.pk3  -> sd:/quake3/missionpack/
 *   classic  fixes/baseq3/zpack-classic.pk3   -> sd:/quake3/baseq3/
 *
 * Makefile.client picks the source pk3 + destination name per flavor and
 * generates bundled_pak_embedded.h; this TU is only compiled for flavors that
 * actually have one (oa/ef ship none).
 */
#include <stdio.h>

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "bundled_pak_embedded.h"

#define WIIU_BUNDLED_PAK_PATH "fs:/vol/external01/quake3/" WIIU_BUNDLED_PAK_NAME

void WiiU_ExtractBundledPak( void ) {
	FILE *f;
	unsigned int existingLen = 0;
	unsigned int existingCsum = 0;
	unsigned char chunk[4096];
	size_t n;

	f = fopen( WIIU_BUNDLED_PAK_PATH, "rb" );
	if ( f ) {
		while ( ( n = fread( chunk, 1, sizeof( chunk ), f ) ) > 0 ) {
			size_t i;
			for ( i = 0; i < n; i++ ) {
				existingCsum += chunk[i];
			}
			existingLen += (unsigned int)n;
		}
		fclose( f );

		if ( existingLen == wiiu_bundled_pak_len && existingCsum == wiiu_bundled_pak_csum ) {
			return; // already up to date
		}
	}

	f = fopen( WIIU_BUNDLED_PAK_PATH, "wb" );
	if ( !f ) {
		return;
	}
	fwrite( wiiu_bundled_pak_data, 1, wiiu_bundled_pak_len, f );
	fclose( f );
}
