void dummy(void *data, Uint8 *stream, int len);
void process_audio(voice_t *voice, sound_t *sounds, int vol, float pitch);
void mix_audio(void *data, Uint8 *stream, int length);
void init_audio(SDL_AudioSpec **audio);
void load_audio(SDL_AudioSpec **audio, sound_t *sound)
void free_audio(SDL_AudioSpec **audio) 
