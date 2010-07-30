sound_t sounds[SOUNDS]
voice_t voices[VOICES]

SDL_AudioSpec spec;


int sm_open(void)
{
	SDL_AudioSpec as;

	memset(sounds, 0, sizeof(sounds));
	memset(voices, 0, sizeof(voices));

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		return -2;

	as.freq = 44100;
	as.format = AUDIO_S16SYS;
	as.channels = 2;
	as.samples = 1024;
	as.callback = sm_mixer;
	if(SDL_OpenAudio(&as, &spec) < 0)
		return -3;

	if(audiospec.format != AUDIO_S16SYS)
		return -4;

	SDL_PauseAudio(0);
	return 0;
}
