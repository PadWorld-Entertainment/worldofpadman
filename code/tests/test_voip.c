#include "test_shared.h"
#include "../client/cl_voip.h"

TESTS_GLOBALS();

static OpusEncoder *encoder;
static OpusDecoder *decoder;
static int16_t pcm[VOIP_MAX_PACKET_SAMPLES];

static void testPacketLimits(void) {
	EXPECT_EQ_INT(qtrue, Voip_ValidPacket(0, 0, 1, 1));
	EXPECT_EQ_INT(qtrue, Voip_ValidPacket(255, INT_MAX - 3, 3, VOIP_MAX_PACKET_BYTES));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(-1, 0, 1, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(256, 0, 1, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, -1, 1, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, INT_MAX, 1, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, 0, 0, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, 0, 4, 1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, 0, 1, -1));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, 0, 1, 0));
	EXPECT_EQ_INT(qfalse, Voip_ValidPacket(1, 0, 1, VOIP_MAX_PACKET_BYTES + 1));
}

static void testCaptureGain(void) {
	int16_t samples[] = {20000, -20000, 1000, -1000, 0};
	const float expectedPower = (2.0f * 20000 * 20000 + 2.0f * 1000 * 1000) *
								 100.0f / (32768.0f * 32768.0f * 5);
	float power = CL_VoipCaptureGain(samples, ARRAY_LEN(samples), 2.0f);
	EXPECT_EQ_INT(32767, samples[0]);
	EXPECT_EQ_INT(-32768, samples[1]);
	EXPECT_EQ_INT(2000, samples[2]);
	EXPECT_EQ_INT(-2000, samples[3]);
	EXPECT_EQ_INT(0, samples[4]);
	EXPECT_EQ_INT(qtrue, fabsf(power - expectedPower) < 0.0001f);
	EXPECT_EQ_INT(0, CL_VoipCaptureGain(samples, 0, 1.0f));

	samples[0] = 1000;
	CL_VoipCaptureGain(samples, 1, NAN);
	EXPECT_EQ_INT(0, samples[0]);
	samples[0] = -1000;
	CL_VoipCaptureGain(samples, 1, INFINITY);
	EXPECT_EQ_INT(0, samples[0]);
}

static void testPacketPower(void) {
	int16_t samples[VOIP_MAX_PACKET_SAMPLES];
	int i;
	for (i = 0; i < ARRAY_LEN(samples); i++) {
		samples[i] = i < VOIP_MAX_FRAME_SAMPLES * 2 ? 5000 : 0;
	}
	// Speech in the first 40 ms must still count when the final 20 ms is silent.
	EXPECT_EQ_INT(qtrue, CL_VoipCaptureGain(samples, ARRAY_LEN(samples), 1.0f) > 0.25f);
}

static void testDecodeTiming(void) {
	byte packet[1024];
	int16_t decoded[VOIP_DECODE_BUFFER_SAMPLES + 1];
	int frames, lost;

	for (frames = 1; frames <= VOIP_MAX_PACKET_FRAMES; frames++) {
		int bytes;
		opus_encoder_ctl(encoder, OPUS_RESET_STATE);
		bytes = opus_encode(encoder, pcm, frames * VOIP_MAX_FRAME_SAMPLES, packet, sizeof(packet));
		ASSERT_EQ_INT(qtrue, bytes > 0);
		for (lost = 0; lost <= VOIP_MAX_PACKET_FRAMES; lost++) {
			int samples;
			opus_decoder_ctl(decoder, OPUS_RESET_STATE);
			// Warm up the decoder before simulating loss in an active stream.
			ASSERT_EQ_INT(frames * VOIP_MAX_FRAME_SAMPLES,
				CL_VoipDecode(decoder, packet, bytes, frames, 0, decoded, VOIP_DECODE_BUFFER_SAMPLES));
			decoded[VOIP_DECODE_BUFFER_SAMPLES] = 12345;
			samples = CL_VoipDecode(decoder, packet, bytes, frames, lost, decoded, VOIP_DECODE_BUFFER_SAMPLES);
			EXPECT_EQ_INT((lost + frames) * VOIP_MAX_FRAME_SAMPLES, samples);
			EXPECT_EQ_INT(12345, decoded[VOIP_DECODE_BUFFER_SAMPLES]);
		}
		// Long gaps resume immediately instead of filling the audio queue with silence.
		EXPECT_EQ_INT(frames * VOIP_MAX_FRAME_SAMPLES,
			CL_VoipDecode(decoder, packet, bytes, frames, INT_MAX, decoded, VOIP_DECODE_BUFFER_SAMPLES));
	}
}

static void testMalformedPackets(void) {
	byte packet[1024];
	byte invalid[] = {0xff, 0xff};
	int16_t decoded[VOIP_DECODE_BUFFER_SAMPLES];
	int bytes = opus_encode(encoder, pcm, VOIP_MAX_FRAME_SAMPLES, packet, sizeof(packet));
	ASSERT_EQ_INT(qtrue, bytes > 0);
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, packet, 0, 1, 0, decoded, ARRAY_LEN(decoded)));
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, packet, -1, 1, 0, decoded, ARRAY_LEN(decoded)));
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, packet, bytes, 0, 0, decoded, ARRAY_LEN(decoded)));
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, packet, bytes, 2, 0, decoded, ARRAY_LEN(decoded)));
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, packet, bytes, 1, -1, decoded, ARRAY_LEN(decoded)));
	EXPECT_EQ_INT(OPUS_INVALID_PACKET, CL_VoipDecode(decoder, invalid, sizeof(invalid), 1, 0, decoded, ARRAY_LEN(decoded)));
	decoded[0] = 12345;
	EXPECT_EQ_INT(OPUS_BUFFER_TOO_SMALL, CL_VoipDecode(decoder, packet, bytes, 1, 1, decoded, VOIP_MAX_FRAME_SAMPLES));
	EXPECT_EQ_INT(12345, decoded[0]);
}

int main(void) {
	int error, i;
	encoder = opus_encoder_create(VOIP_SAMPLE_RATE, 1, OPUS_APPLICATION_VOIP, &error);
	if (!encoder) {
		return 1;
	}
	decoder = opus_decoder_create(VOIP_SAMPLE_RATE, 1, &error);
	if (!decoder) {
		opus_encoder_destroy(encoder);
		return 1;
	}
	for (i = 0; i < ARRAY_LEN(pcm); i++) {
		pcm[i] = (int16_t)(10000 * sinf(2.0f * M_PI * 440 * i / VOIP_SAMPLE_RATE));
	}

	ADD_TEST(testPacketLimits);
	ADD_TEST(testCaptureGain);
	ADD_TEST(testPacketPower);
	ADD_TEST(testDecodeTiming);
	ADD_TEST(testMalformedPackets);

	opus_decoder_destroy(decoder);
	opus_encoder_destroy(encoder);
	TESTS_SHUTDOWN();
}
