/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of WorldOfPadman source code.

WorldOfPadman source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

WorldOfPadman source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with WorldOfPadman source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// cl_voip.c -- VoIP jitter buffer and network adaptation

#include "client.h"
#include "cl_voip.h"
#include <opus.h>

#ifdef USE_VOIP

cvar_t *cl_voipJitterDelay;

// ============================================================================
// Jitter Buffer
// ============================================================================

/*
==================
CL_VoipJitterInit

Initialize a jitter buffer to empty state
==================
*/
void CL_VoipJitterInit(voipJitterBuffer_t *jb) {
	Com_Memset(jb, 0, sizeof(*jb));
	jb->delayMs = VOIP_JITTER_DEFAULT_MS;
	jb->nextPlaySeq = -1;
	jb->lastInsertSeq = -1;
	jb->active = qfalse;
}

/*
==================
CL_VoipJitterReset

Reset the jitter buffer, discarding all buffered packets
==================
*/
void CL_VoipJitterReset(voipJitterBuffer_t *jb) {
	int i;
	for (i = 0; i < VOIP_JITTER_QUEUE; i++) {
		jb->packets[i].used = qfalse;
	}
	jb->count = 0;
	jb->nextPlaySeq = -1;
	jb->lastInsertSeq = -1;
	jb->active = qfalse;
}

/*
==================
CL_VoipJitterInsert

Insert a packet into the jitter buffer, ordered by sequence number.
Returns qtrue if the packet was accepted, qfalse if dropped (duplicate, too old, or buffer full).
==================
*/
qboolean CL_VoipJitterInsert(voipJitterBuffer_t *jb, int sequence, int generation,
                             int frames, int flags, const byte *data, int dataLen) {
	int i;
	int oldestIdx = -1;
	int oldestSeq = INT_MAX;

	// Generation change - reset the buffer
	if (jb->active && generation != jb->generation) {
		CL_VoipJitterReset(jb);
	}

	jb->generation = generation;

	// Check for duplicate
	for (i = 0; i < VOIP_JITTER_QUEUE; i++) {
		if (jb->packets[i].used && jb->packets[i].sequence == sequence) {
			return qfalse; // duplicate
		}
	}

	// If we already played past this sequence, drop it
	if (jb->nextPlaySeq >= 0 && sequence < jb->nextPlaySeq) {
		Com_DPrintf("VoIP Jitter: dropping late packet seq %d (next play: %d)\n",
		            sequence, jb->nextPlaySeq);
		return qfalse;
	}

	// Validate data length
	if (dataLen <= 0 || dataLen > (int)sizeof(jb->packets[0].data)) {
		return qfalse;
	}

	// Find a free slot, or the oldest packet to evict
	for (i = 0; i < VOIP_JITTER_QUEUE; i++) {
		if (!jb->packets[i].used) {
			// Found a free slot
			break;
		}
		if (jb->packets[i].sequence < oldestSeq) {
			oldestSeq = jb->packets[i].sequence;
			oldestIdx = i;
		}
	}

	// Buffer full - evict oldest
	if (i == VOIP_JITTER_QUEUE) {
		if (oldestIdx >= 0) {
			i = oldestIdx;
			jb->count--;
			Com_DPrintf("VoIP Jitter: buffer full, evicting seq %d\n", oldestSeq);
		} else {
			return qfalse; // shouldn't happen
		}
	}

	// Store the packet
	jb->packets[i].sequence = sequence;
	jb->packets[i].generation = generation;
	jb->packets[i].frames = frames;
	jb->packets[i].flags = flags;
	jb->packets[i].dataLen = dataLen;
	Com_Memcpy(jb->packets[i].data, data, dataLen);
	jb->packets[i].used = qtrue;
	jb->count++;

	if (sequence > jb->lastInsertSeq) {
		jb->lastInsertSeq = sequence;
	}

	// Start the buffer on first packet
	if (!jb->active) {
		jb->active = qtrue;
		jb->nextPlaySeq = sequence;
		jb->lastPlayTime = cls.realtime;
	}

	return qtrue;
}

/*
==================
CL_VoipJitterReady

Check if the jitter buffer has accumulated enough delay to start playback.
Uses the adaptive jitter delay if network adaptation is enabled.
==================
*/
qboolean CL_VoipJitterReady(voipJitterBuffer_t *jb, int currentTime) {
	int delayMs;

	if (!jb->active || jb->count == 0) {
		return qfalse;
	}

	// Use cvar if available, otherwise default
	delayMs = cl_voipJitterDelay ? cl_voipJitterDelay->integer : VOIP_JITTER_DEFAULT_MS;
	if (delayMs < 0) {
		delayMs = 0;
	}

	// If we've already started playing, keep going (don't re-buffer)
	if (jb->nextPlaySeq > jb->packets[0].sequence || jb->count > 1) {
		// Check if enough time has passed since we started buffering
		if (currentTime - jb->lastPlayTime >= delayMs) {
			return qtrue;
		}
	}

	// If buffer has enough packets, start playing
	// Each packet is ~60ms (VOIP_MAX_PACKET_FRAMES * 20ms)
	if (jb->count >= (delayMs / 60) + 1) {
		return qtrue;
	}

	return qfalse;
}

/*
==================
CL_VoipJitterGet

Retrieve the next packet in sequence order from the jitter buffer.
Returns qtrue if a packet was retrieved, qfalse if none available.
==================
*/
qboolean CL_VoipJitterGet(voipJitterBuffer_t *jb, int *sequence, int *frames,
                          int *flags, byte *data, int *dataLen, int currentTime) {
	int i;
	int bestIdx = -1;
	int bestSeq = INT_MAX;

	if (!jb->active || jb->count == 0) {
		return qfalse;
	}

	// Find the packet with the lowest sequence number >= nextPlaySeq
	for (i = 0; i < VOIP_JITTER_QUEUE; i++) {
		if (!jb->packets[i].used) {
			continue;
		}
		if (jb->packets[i].sequence >= jb->nextPlaySeq && jb->packets[i].sequence < bestSeq) {
			bestSeq = jb->packets[i].sequence;
			bestIdx = i;
		}
	}

	if (bestIdx < 0) {
		// No packet at or after nextPlaySeq - stream might be done
		jb->active = qfalse;
		return qfalse;
	}

	// Return the packet data
	*sequence = jb->packets[bestIdx].sequence;
	*frames = jb->packets[bestIdx].frames;
	*flags = jb->packets[bestIdx].flags;
	*dataLen = jb->packets[bestIdx].dataLen;
	Com_Memcpy(data, jb->packets[bestIdx].data, jb->packets[bestIdx].dataLen);

	// Mark as consumed
	jb->packets[bestIdx].used = qfalse;
	jb->count--;

	// Advance play cursor past this packet's frames
	jb->nextPlaySeq = *sequence + *frames;
	jb->lastPlayTime = currentTime;

	return qtrue;
}

// ============================================================================
// Network Statistics
// ============================================================================

/*
==================
CL_VoipNetStatsInit

Initialize network stats for a sender
==================
*/
void CL_VoipNetStatsInit(voipNetStats_t *stats) {
	Com_Memset(stats, 0, sizeof(*stats));
	stats->lastReceivedSeq = -1;
	stats->windowStartSeq = -1;
	stats->lastArrivalTime = 0;
	stats->adaptedBitrate = 0; // 0 means "use cvar default"
	stats->adaptedLossPercent = -1;
	stats->adaptedJitterMs = VOIP_JITTER_DEFAULT_MS;
}

/*
==================
CL_VoipNetStatsReset

Reset network stats (e.g., on generation change)
==================
*/
void CL_VoipNetStatsReset(voipNetStats_t *stats) {
	int savedBitrate = stats->adaptedBitrate;
	int savedJitter = stats->adaptedJitterMs;
	CL_VoipNetStatsInit(stats);
	// Preserve adapted values across resets so we don't spike on new generations
	stats->adaptedBitrate = savedBitrate;
	stats->adaptedJitterMs = savedJitter;
}

/*
==================
CL_VoipNetStatsPacketReceived

Record that a packet was received from a sender. Tracks loss and jitter.
Called from CL_ParseVoip for each accepted packet.
==================
*/
void CL_VoipNetStatsPacketReceived(voipNetStats_t *stats, int sequence, int frames, int currentTime) {
	int arrivalDelta;
	float alpha;

	stats->packetsReceived++;

	// Track packet loss by comparing sequence numbers
	if (stats->lastReceivedSeq >= 0) {
		int expected = sequence - stats->lastReceivedSeq;
		if (expected > 0) {
			int lost = expected - frames; // frames we should have gotten but didn't
			if (lost < 0) lost = 0;
			stats->packetsLost += lost;
			stats->packetsExpected += expected;
		}
	} else {
		stats->windowStartSeq = sequence;
	}

	stats->lastReceivedSeq = sequence + frames - 1;

	// Calculate smoothed loss rate using exponential moving average
	if (stats->packetsExpected > 0) {
		float instantLoss = (float)stats->packetsLost / (float)stats->packetsExpected;
		// Smooth with alpha=0.1 for stability
		alpha = 0.1f;
		stats->lossRate = stats->lossRate * (1.0f - alpha) + instantLoss * alpha;
	}

	// Track inter-arrival jitter
	if (stats->lastArrivalTime > 0) {
		arrivalDelta = currentTime - stats->lastArrivalTime;

		// Store in circular buffer
		stats->arrivalDeltas[stats->deltaIndex] = arrivalDelta;
		stats->deltaIndex = (stats->deltaIndex + 1) % VOIP_STATS_WINDOW;
		if (stats->deltaCount < VOIP_STATS_WINDOW) {
			stats->deltaCount++;
		}

		// Calculate jitter as mean deviation from expected interval
		// Expected interval is ~60ms per packet (VOIP_MAX_PACKET_FRAMES * 20ms)
		if (stats->deltaCount > 1) {
			int j;
			float meanDelta = 0.0f;
			float variance = 0.0f;

			for (j = 0; j < stats->deltaCount; j++) {
				meanDelta += (float)stats->arrivalDeltas[j];
			}
			meanDelta /= (float)stats->deltaCount;

			for (j = 0; j < stats->deltaCount; j++) {
				float diff = (float)stats->arrivalDeltas[j] - meanDelta;
				variance += diff * diff;
			}
			variance /= (float)stats->deltaCount;

			// Jitter is the standard deviation of inter-arrival times
			// Use a simple approximation: sqrt via Newton's method iteration
			{
				float jitter = variance;
				if (jitter > 0.0f) {
					// Two iterations of Newton's method for sqrt
					float x = jitter;
					x = 0.5f * (x + jitter / x);
					x = 0.5f * (x + jitter / x);
					jitter = x;
				}
				// Smooth jitter estimate
				alpha = 0.15f;
				stats->jitterMs = stats->jitterMs * (1.0f - alpha) + jitter * alpha;
			}
		}
	}

	stats->lastArrivalTime = currentTime;

	// Reset window counters periodically to keep stats fresh
	if (stats->packetsExpected > VOIP_STATS_WINDOW * 2) {
		stats->packetsLost = (int)(stats->lossRate * VOIP_STATS_WINDOW);
		stats->packetsExpected = VOIP_STATS_WINDOW;
		stats->packetsReceived = VOIP_STATS_WINDOW - stats->packetsLost;
	}
}

// ============================================================================
// Adaptive Network Response
// ============================================================================

/*
==================
CL_VoipAdaptToNetwork

Periodically called to adapt encoder settings and jitter buffer depth
based on observed network conditions from all active senders.
==================
*/
void CL_VoipAdaptToNetwork(void) {
	int i;
	float worstLoss = 0.0f;
	float worstJitter = 0.0f;
	int activeSenders = 0;
	int newLossPercent;
	int newBitrate;
	int newJitterMs;
	int currentTime = cls.realtime;
	static int lastAdaptTime = 0;

	if (!clc.voipCodecInitialized) {
		return;
	}

	if (!cl_voipAdaptive || !cl_voipAdaptive->integer) {
		return;
	}

	// Only adapt once per second to avoid thrashing
	if (currentTime - lastAdaptTime < VOIP_ADAPT_INTERVAL_MS) {
		return;
	}
	lastAdaptTime = currentTime;

	// Aggregate stats across all active senders
	for (i = 0; i < MAX_CLIENTS; i++) {
		voipNetStats_t *stats = &clc.voipSenders[i].netStats;

		// Only consider senders with recent data
		if (stats->packetsReceived == 0 || stats->lastArrivalTime == 0) {
			continue;
		}

		// Skip stale stats (no packet in last 5 seconds)
		if (currentTime - stats->lastArrivalTime > 5000) {
			continue;
		}

		activeSenders++;

		if (stats->lossRate > worstLoss) {
			worstLoss = stats->lossRate;
		}
		if (stats->jitterMs > worstJitter) {
			worstJitter = stats->jitterMs;
		}
	}

	if (activeSenders == 0) {
		return;
	}

	// --- Adapt encoder packet loss percentage ---
	// Convert loss rate to integer percentage for Opus
	newLossPercent = (int)(worstLoss * 100.0f + 0.5f);
	if (newLossPercent < 0) newLossPercent = 0;
	if (newLossPercent > 100) newLossPercent = 100;

	// Only update if changed significantly (avoid constant ctl calls)
	if (newLossPercent != clc.voipSenders[0].netStats.adaptedLossPercent) {
		opus_encoder_ctl(clc.opusEncoder, OPUS_SET_PACKET_LOSS_PERC(newLossPercent));
		Com_DPrintf("VoIP Adapt: packet loss -> %d%%\n", newLossPercent);

		// Store adapted value across all sender slots
		for (i = 0; i < MAX_CLIENTS; i++) {
			clc.voipSenders[i].netStats.adaptedLossPercent = newLossPercent;
		}
	}

	// --- Adapt bitrate based on loss ---
	// Under high loss, reduce bitrate to leave room for FEC redundancy
	// Under low loss, allow higher bitrate for quality
	{
		int baseBitrate = cl_voipBitrate->integer;

		if (worstLoss > 0.15f) {
			// High loss: reduce to 60% of base
			newBitrate = (int)(baseBitrate * 0.6f);
		} else if (worstLoss > 0.05f) {
			// Moderate loss: reduce to 80% of base
			newBitrate = (int)(baseBitrate * 0.8f);
		} else {
			// Low loss: use full base bitrate
			newBitrate = baseBitrate;
		}

		// Clamp to bounds
		if (newBitrate < VOIP_MIN_BITRATE) newBitrate = VOIP_MIN_BITRATE;
		if (newBitrate > VOIP_MAX_BITRATE) newBitrate = VOIP_MAX_BITRATE;

		if (newBitrate != clc.voipSenders[0].netStats.adaptedBitrate) {
			opus_encoder_ctl(clc.opusEncoder, OPUS_SET_BITRATE(newBitrate));
			Com_DPrintf("VoIP Adapt: bitrate -> %d bps (loss: %.1f%%)\n",
			            newBitrate, worstLoss * 100.0f);

			for (i = 0; i < MAX_CLIENTS; i++) {
				clc.voipSenders[i].netStats.adaptedBitrate = newBitrate;
			}
		}
	}

	// --- Adapt jitter buffer depth ---
	// Target: 2x observed jitter, clamped to reasonable bounds
	newJitterMs = (int)(worstJitter * 2.0f + 0.5f);
	if (newJitterMs < VOIP_MIN_JITTER_MS) newJitterMs = VOIP_MIN_JITTER_MS;
	if (newJitterMs > VOIP_MAX_JITTER_MS) newJitterMs = VOIP_MAX_JITTER_MS;

	// Don't shrink too aggressively - only shrink by 10ms at a time
	{
		int currentDelay = cl_voipJitterDelay ? cl_voipJitterDelay->integer : VOIP_JITTER_DEFAULT_MS;
		if (newJitterMs < currentDelay - 10) {
			newJitterMs = currentDelay - 10;
		}
	}

	if (newJitterMs != clc.voipSenders[0].netStats.adaptedJitterMs) {
		Cvar_Set("cl_voipJitterDelay", va("%d", newJitterMs));
		Com_DPrintf("VoIP Adapt: jitter buffer -> %d ms (jitter: %.1f ms)\n",
		            newJitterMs, worstJitter);

		for (i = 0; i < MAX_CLIENTS; i++) {
			clc.voipSenders[i].netStats.adaptedJitterMs = newJitterMs;
		}
	}
}

// ============================================================================
// User Experience Commands
// ============================================================================

/*
==================
CL_VoipPrintStats

Print VoIP network statistics to the console.
==================
*/
void CL_VoipPrintStats(void) {
	int i;
	int printed = 0;

	Com_Printf("VoIP Network Stats:\n");
	Com_Printf("  Adaptive: %s\n", cl_voipAdaptive->integer ? "ON" : "OFF");
	Com_Printf("  Jitter buffer: %d ms\n", cl_voipJitterDelay->integer);
	Com_Printf("  Encoder bitrate: %d bps, FEC: %s, loss hint: %d%%\n",
	           cl_voipBitrate->integer, cl_voipFEC->integer ? "on" : "off",
	           cl_voipPacketLossRate->integer);
	Com_Printf("  Test mode: %s\n", clc.voipTestMode ? "ACTIVE" : "off");
	Com_Printf("  ---\n");

	for (i = 0; i < MAX_CLIENTS; i++) {
		voipNetStats_t *stats = &clc.voipSenders[i].netStats;
		if (stats->packetsReceived == 0) {
			continue;
		}
		Com_Printf("  Client #%d: loss=%.1f%% jitter=%.1f ms pkts=%d\n",
		           i, stats->lossRate * 100.0f, stats->jitterMs,
		           stats->packetsReceived);
		if (stats->adaptedBitrate > 0) {
			Com_Printf("    adapted: bitrate=%d loss_hint=%d%% jitter_buf=%d ms\n",
			           stats->adaptedBitrate, stats->adaptedLossPercent,
			           stats->adaptedJitterMs);
		}
		printed++;
	}

	if (!printed) {
		Com_Printf("  No active VoIP senders.\n");
	}
}

// ============================================================================
// Adaptive Voice Activity Detection
// ============================================================================

/*
==================
CL_VoipVADCheck

Adaptive VAD that tracks the noise floor and triggers when the signal
is significantly above it. This works out of the box for any mic level
without requiring manual threshold tuning.

Returns qtrue if the power level indicates speech.
==================
*/
qboolean CL_VoipVADCheck(float power) {
	static float noiseFloor = 0.0f;
	static float speechLevel = 0.0f;
	static int holdFrames = 0;
	float threshold;
	float manualThreshold = cl_voipVADThreshold->value;

	// If manual threshold is set to 0, always transmit (no VAD)
	if (manualThreshold <= 0.0f) {
		return qtrue;
	}

	// Slowly adapt noise floor downward, quickly adapt upward only slightly
	// This tracks the ambient noise level
	if (power < noiseFloor || noiseFloor == 0.0f) {
		// Signal is below current noise floor — adapt quickly downward
		noiseFloor = noiseFloor * 0.9f + power * 0.1f;
	} else if (power < noiseFloor * 2.0f) {
		// Signal is near noise floor — adapt slowly upward
		noiseFloor = noiseFloor * 0.995f + power * 0.005f;
	}
	// If signal is well above noise floor, don't adapt (it's speech)

	// Threshold is noise floor + a margin based on the manual threshold setting.
	// Manual threshold acts as sensitivity: lower = more sensitive.
	// At default 0.05: threshold = noiseFloor + 0.05 (very sensitive)
	// At 0.5: threshold = noiseFloor + 0.5 (less sensitive, needs louder speech)
	threshold = noiseFloor + manualThreshold;

	// Also enforce a minimum absolute threshold to avoid triggering on near-silence
	if (threshold < 0.01f) {
		threshold = 0.01f;
	}

	if (power >= threshold) {
		// Speech detected — hold for a few frames to avoid cutting off tails
		holdFrames = 8; // ~500ms at 4Hz packet rate (8 * 60ms)
		speechLevel = speechLevel * 0.7f + power * 0.3f;
		return qtrue;
	}

	// Hold period: keep transmitting briefly after speech stops
	if (holdFrames > 0) {
		holdFrames--;
		return qtrue;
	}

	return qfalse;
}

/*
==================
CL_VoipCodecInit

Initialize the Opus encoder and decoders. Safe to call multiple times —
does nothing if already initialized.
==================
*/
void CL_VoipCodecInit(void) {
	int i;
	int error;

	if (clc.voipCodecInitialized) {
		return;
	}

	clc.opusEncoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &error);
	if (error) {
		Com_Printf("VoIP: Error opus_encoder_create %d\n", error);
		return;
	}

	// Configure Opus encoder for VoIP quality
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_BITRATE(cl_voipBitrate->integer));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_COMPLEXITY(cl_voipComplexity->integer));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_VBR(1));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_VBR_CONSTRAINT(1));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_DTX(1));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_INBAND_FEC(cl_voipFEC->integer));
	opus_encoder_ctl(clc.opusEncoder, OPUS_SET_PACKET_LOSS_PERC(cl_voipPacketLossRate->integer));

	Com_DPrintf("VoIP: Opus encoder configured - bitrate:%d complexity:%d fec:%d loss:%d%%\n",
	            cl_voipBitrate->integer, cl_voipComplexity->integer,
	            cl_voipFEC->integer, cl_voipPacketLossRate->integer);

	for (i = 0; i < MAX_CLIENTS; i++) {
		clc.opusDecoder[i] = opus_decoder_create(48000, 1, &error);
		if (error) {
			Com_Printf("VoIP: Error opus_decoder_create(%d) %d\n", i, error);
			return;
		}
		clc.voipSenders[i].ignore = qfalse;
		clc.voipSenders[i].gain = 1.0f;
		clc.voipSenders[i].lastPacketTime = 0;
		CL_VoipJitterInit(&clc.voipSenders[i].jitter);
		CL_VoipNetStatsInit(&clc.voipSenders[i].netStats);
	}

	clc.voipCodecInitialized = qtrue;
	clc.voipMuteAll = qfalse;
	Com_Memset(clc.voipTargets, ~0, sizeof(clc.voipTargets));

	// Check if capture device is available and expose to UI
	if (!S_CaptureAvailable()) {
		Com_Printf("VoIP: no capture device available, VoIP transmit disabled.\n");
		Cvar_Set("cl_voipNoMic", "1");
	} else {
		Cvar_Set("cl_voipNoMic", "0");
	}

	Com_Printf("VoIP: codec initialized.\n");
}

/*
==================
CL_VoipToggleTest

Toggle microphone loopback test mode. When active, captured audio is
encoded and immediately decoded and played back locally, allowing the
user to hear their own microphone quality without a server.
==================
*/
void CL_VoipToggleTest(void) {
	if (clc.voipTestMode) {
		clc.voipTestMode = qfalse;
		S_StopCapture();
		S_MasterGain(1.0f);
		Cvar_Set("cl_voipTestMode", "0");
		Cvar_Set("cl_voipMicLevel", "0");
		Com_Printf("VoIP: microphone test stopped.\n");
	} else {
		// Initialize codec on demand if not already done
		if (!clc.voipCodecInitialized) {
			CL_VoipCodecInit();
			if (!clc.voipCodecInitialized) {
				Com_Printf("VoIP: failed to initialize codec for test.\n");
				return;
			}
		}
		clc.voipTestMode = qtrue;
		Cvar_Set("cl_voipTestMode", "1");
		S_MasterGain(Com_Clamp(0.0f, 1.0f, cl_voipGainDuringCapture->value));
		S_StartCapture();
		Com_Printf("VoIP: microphone test started. Speak into your mic.\n");
		Com_Printf("VoIP: type 'voip test' again to stop.\n");
	}
}

/*
==================
CL_VoipTestFrame

Called each frame when test mode is active. Captures audio, encodes it,
decodes it, and plays it back locally.
==================
*/
void CL_VoipTestFrame(void) {
	static int16_t sampbuffer[VOIP_MAX_PACKET_SAMPLES];
	static short decoded[VOIP_DECODE_BUFFER_SAMPLES];
	byte encoded[1024];
	int samples, bytes, numSamples;
	float audioMult;

	if (!clc.voipTestMode || !clc.voipCodecInitialized) {
		return;
	}

	samples = S_AvailableCaptureSamples();
	if (samples < VOIP_MAX_FRAME_SAMPLES) {
		return;
	}

	// Use one frame (20ms) for responsive level metering
	if (samples > VOIP_MAX_PACKET_SAMPLES) {
		samples = VOIP_MAX_PACKET_SAMPLES;
	}
	samples -= samples % VOIP_MAX_FRAME_SAMPLES;
	if (samples <= 0) {
		return;
	}

	audioMult = cl_voipCaptureMult->value;

	S_Capture(samples, (byte *)sampbuffer);

	// Apply gain with clipping protection
	{
		int i;
		float voipPower = 0.0f;
		float peakAmplitude = 0.0f;
		for (i = 0; i < samples; i++) {
			const float flsamp = (float)sampbuffer[i];
			const float absSamp = (flsamp < 0) ? -flsamp : flsamp;
			voipPower += flsamp * flsamp;
			if (absSamp > peakAmplitude) peakAmplitude = absSamp;
			sampbuffer[i] = CL_VoipClampSample(flsamp * audioMult);
		}
		clc.voipPower = (voipPower / (32768.0f * 32768.0f * ((float)samples))) * 100.0f;
		// Store peak amplitude as percentage of full scale (0-100) for UI display
		Cvar_Set("cl_voipMicLevel", va("%d", (int)((peakAmplitude / 32768.0f) * 100.0f)));
	}

	// Encode
	bytes = opus_encode(clc.opusEncoder, sampbuffer, samples, encoded, sizeof(encoded));
	if (bytes <= 0) {
		return;
	}

	// Decode using our own decoder (slot 0 reserved for loopback in test mode)
	numSamples = opus_decode(clc.opusDecoder[clc.clientNum >= 0 ? clc.clientNum : 0],
	                         encoded, bytes, decoded, VOIP_DECODE_BUFFER_SAMPLES, 0);
	if (numSamples <= 0) {
		return;
	}

	// Play back locally (use a dedicated raw sample stream)
	S_RawSamples(0, numSamples, 48000, 2, 1, (const byte *)decoded, 1.0f, -1);
}

#endif // USE_VOIP
