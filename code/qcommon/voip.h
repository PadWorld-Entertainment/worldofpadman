/*
 * VoIP wire limits. Sequence numbers count 20 ms frames, not packets.
 */
#ifndef VOIP_H
#define VOIP_H

#include "q_shared.h"

#define VOIP_SAMPLE_RATE 48000
#define VOIP_MAX_FRAME_SAMPLES (20 * 48)
#define VOIP_MAX_PACKET_FRAMES 3
#define VOIP_MAX_PACKET_SAMPLES (VOIP_MAX_FRAME_SAMPLES * VOIP_MAX_PACKET_FRAMES)
#define VOIP_MAX_PACKET_BYTES 4000

static ID_INLINE qboolean Voip_ValidPacket(int generation, int sequence, int frames, int packetsize) {
	return generation >= 0 && generation <= 255 && frames > 0 && frames <= VOIP_MAX_PACKET_FRAMES &&
		   sequence >= 0 && sequence <= INT_MAX - frames && packetsize > 0 && packetsize <= VOIP_MAX_PACKET_BYTES;
}

#endif
