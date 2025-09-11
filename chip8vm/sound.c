#include "chip8vm/sound.h"

#include <SDL_audio.h>
#include <stdio.h>

static SDL_AudioDeviceID audio_device;
static bool audio_initialized = false;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len)
{
	static int phase = 0;
	int freq = 440;
	int volume = 64;

	for (int i = 0; i < len; i++) {
		stream[i] = ((phase++ / (44100 / freq / 2)) % 2) ? volume : -volume;
	}
}

bool c8vm_sound_create()
{
	if (audio_initialized) {
		return true;
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
		printf("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
		return false;
	}

	return audio_initialized = true;
}

void c8vm_sound_destroy()
{
	if (audio_initialized) {
		SDL_CloseAudioDevice(audio_device);
		audio_initialized = false;
	}
}

void c8vm_sound_start()
{
	if (audio_initialized) {
		SDL_PauseAudioDevice(audio_device, 0);
	}
}

void c8vm_sound_stop()
{
	if (audio_initialized) {
		SDL_PauseAudioDevice(audio_device, 1);
	}
}
