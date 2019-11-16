#pragma once

enum {
	AUDIO_WAVEFORM_PULSE_12_5,
	AUDIO_WAVEFORM_PULSE_25,
	AUDIO_WAVEFORM_PULSE_50,
	AUDIO_WAVEFORM_PULSE_75,
	AUDIO_WAVEFORM_PULSE_MAX,
};

int audioInit();
void audioWaveform(int _waveform);
void audioPlay();
void audioStop();