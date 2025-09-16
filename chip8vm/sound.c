#include "chip8vm/sound.h"

#include <SDL_audio.h>

#include <stdio.h>
#include <stdbool.h>

static SDL_AudioDeviceID audio_device;
static bool audio_initialized;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len)
{
	(void)userdata;
	static int phase = 0;
	int freq = 440;
	int volume = 64;

	for (int i = 0; i < len; i++) {
		int sample = ((phase++ / (44100 / freq / 2)) % 2) ? (128 + volume) : (128 - volume);
		stream[i] = (Uint8)sample;
	}
}

int c8vm_sound_init(void)
{
	if (audio_initialized) {
		return 0;
	}

	SDL_AudioSpec want, have;
	SDL_zero(want);
	want.freq = 44100;
	want.format = AUDIO_S8;
	want.channels = 1;
	want.samples = 512;
	want.callback = sdl_audio_callback;

	audio_device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

	if (audio_device == 0) {
		fprintf(stderr, "SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
		return -1;
	}

	audio_initialized = true;
	return 0;
}

void c8vm_sound_destroy(void)
{
	if (audio_initialized) {
		SDL_CloseAudioDevice(audio_device);
		audio_initialized = false;
	}
}

void c8vm_sound_start(void)
{
	if (audio_initialized) {
		SDL_PauseAudioDevice(audio_device, 0);
	}
}

void c8vm_sound_stop(void)
{
	if (audio_initialized) {
		SDL_PauseAudioDevice(audio_device, 1);
	}
}
