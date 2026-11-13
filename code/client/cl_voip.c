#include "cl_voip.h"

float CL_VoipCaptureGain(int16_t *samples, int count, float gain) {
	float power = 0.0f;
	int i;

	for (i = 0; i < count; i++) {
		const float sample = samples[i];
		float amplified = sample * gain;
		power += sample * sample;
		if (!isfinite(amplified)) {
			amplified = 0.0f;
		} else if (amplified > 32767.0f) {
			amplified = 32767.0f;
		} else if (amplified < -32768.0f) {
			amplified = -32768.0f;
		}
		samples[i] = (int16_t)amplified;
	}

	return count > 0 ? power * (100.0f / (32768.0f * 32768.0f)) / count : 0.0f;
}

int CL_VoipDecode(OpusDecoder *decoder, const byte *packet, int size, int frames, int lostFrames,
				 int16_t *decoded, int capacity) {
	int written = 0;
	int samples;

	if (!Voip_ValidPacket(0, 0, frames, size) || lostFrames < 0 ||
		opus_packet_get_nb_samples(packet, size, VOIP_SAMPLE_RATE) != frames * VOIP_MAX_FRAME_SAMPLES) {
		return OPUS_INVALID_PACKET;
	}

	// Long gaps are resynchronized instead of adding a growing backlog of synthetic audio.
	if (lostFrames > VOIP_MAX_PACKET_FRAMES) {
		lostFrames = 0;
		opus_decoder_ctl(decoder, OPUS_RESET_STATE);
	}
	if (capacity < (lostFrames + frames) * VOIP_MAX_FRAME_SAMPLES) {
		return OPUS_BUFFER_TOO_SMALL;
	}

	if (lostFrames > 0) {
		written = opus_decode(decoder, NULL, 0, decoded, lostFrames * VOIP_MAX_FRAME_SAMPLES, 0);
		if (written < 0) {
			return written;
		}
	}

	samples = opus_decode(decoder, packet, size, decoded + written, capacity - written, 0);
	return samples < 0 ? samples : written + samples;
}
