#ifndef CL_VOIP_H
#define CL_VOIP_H

#include "../qcommon/voip.h"
#include <opus.h>

// Space for one lost 60 ms packet followed by one received 60 ms packet.
#define VOIP_DECODE_BUFFER_SAMPLES (VOIP_MAX_PACKET_SAMPLES * 2)

float CL_VoipCaptureGain(int16_t *samples, int count, float gain);
int CL_VoipDecode(OpusDecoder *decoder, const byte *packet, int size, int frames, int lostFrames,
				 int16_t *decoded, int capacity);

#endif
