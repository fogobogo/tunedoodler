/* voice_t voice[] is global */

/* dummy callback. */
void
dummy(void *data, Uint8 *stream, int len) 
{
}

void
process_audio(voice_t *voice, sound_t *sounds, int vol, float pitch)
{
    voice->data = NULL; /* what ? */

    /* process audio data */
    voice->length = sounds->length / 2;
    voice->pos = 0;
    voice->f_pos = 0;
	/* pump up the volume! pump it up! pump it up! */
	voice->vol = (int)vol * 256;
	printf("voice->vol: %d\n",voice->vol);
    voice->pitch = pitch;

	/* point the channel to the sound data ? */
    voice->data = (Sint16 *)sounds->data;
}

void
mix_audio(void *data, Uint8 *stream, int length)
{
    int s;
    int i;
    Sint16 *buffer;
    voice_t *v;

    /* buffer points to stream(?), why not just manipulate the stream directly ? */
	/* init buffer */
    buffer = (Sint16 *)stream;
	/* clearing the buffer, no idea why (artifacts?) */
	/* only the buffer gets played back? */
	memset(buffer, 0, length); /* not sure that is a good idea */

    /* how come? */
    length = length / 2;

    /* for every channel (voice) ... */
    for(i=0; i < 10; i++) {
		v = &voice[i];
		/* if there is no data start the next cycle */
		if(!v->data) { continue; }
			
		/* roll over the samples */
		/* multiply length by two because... ??? */
		for(s = 0; s < length; ++s) {
			/* WHAT DOES THIS DO ?? */
			if(v->pos >= v->length) {
				v->data = NULL;
				/* printf("ding.\n"); */
				break;
			}

			v->pos = (int)v->f_pos; /* float to int */
			/* bitshift to punch back the data to the Uint8 format ? */
			/* fill buffer */
			buffer[s++] += v->data[v->pos] * v->vol >> 8; /* s = +1 */
			buffer[s++] += v->data[v->pos] * v->vol >> 8; /* s = +2 */
			/* printf("dong.\n"); */
			v->f_pos += v->pitch;
		}
    }
}

void
init_audio(SDL_AudioSpec **audio)
{
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stdout,"could not initalize audio... :<\n");
    }

    /* TODO: find out why simplemixer doesnt need that */
    (*audio) = malloc(sizeof(SDL_AudioSpec));

    (*audio)->freq = 44100;
    (*audio)->format = AUDIO_S16SYS;
    (*audio)->channels = 2; /* stereo */
    (*audio)->samples = 8192; /* big buffer, we won't need a quick response */
    (*audio)->callback = mix_audio;
    (*audio)->userdata = NULL;

    if(SDL_OpenAudio((*audio),NULL) != 0) {
        fprintf(stdout,"could not obtain audio... :<\n");
        exit(1);
    }

	printf("freq: %d\n",(*audio)->freq);
	printf("format: %d\n",(*audio)->format);
	printf("channels: %d\n",(*audio)->channels);
	printf("samples: %d\n",(*audio)->samples);

    /* from this point on data gets passed to the soundcard */
    SDL_PauseAudio(0);
}

void
load_audio(SDL_AudioSpec **audio, sound_t *sound)
{
    SDL_LoadWAV("default/file_complete.wav",(*audio),&sound[0].data,&sound[0].length);
    SDL_LoadWAV("default/808-cowbell.wav",(*audio),&sound[1].data,&sound[1].length);
    SDL_LoadWAV("default/808-clap.wav",(*audio),&sound[2].data,&sound[2].length);
    SDL_LoadWAV("default/808-hihat.wav",(*audio),&sound[3].data,&sound[3].length);
}

void
free_audio(SDL_AudioSpec **audio) 
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free((*audio));
}

