/* voice_t voice[] is global */

/* dummy callback. */
void
dummy(void *data, Uint8 *stream, int len) 
{
}

void
process_audio(voice_t *voice, sound_t *sounds, float l_vol, float r_vol, float pitch)
{
    voice->data = NULL;

    /* process audio data */
    voice->length = sounds->length / 2;
    voice->pos = 0;
    voice->f_pos = 0;
    /* is that needed? */
    voice->l_vol = (int)(l_vol * 256.0); /* bump the volume since data is now Sint16 ?? */
    voice->r_vol = (int)(r_vol * 256.0);
    voice->pitch = pitch;

    voice->data = (Sint16 *)sounds->data;
}

void
mixer(void *data, Uint8 *stream, int length)
{
    int s;
    Sint16 *buffer;
    voice_t *v;

    buffer = (Sint16 *)stream;
	/* clearing the buffer is for pussies! */
	memset(buffer, 0, length);

    /* how come? */
    length = length / 4;

    /* this. it needs to be fixed */
    v = &voice[0];
    /* if there is no data continue?? */
    /*
    if(!v->data) { continue; }
    */

    for(s = 0; s < length * 2; ++s) {
        /* WHAT DOES THIS DO ?? */
        if(v->pos >= v->length) {
            v->data = NULL;
            printf("ding.\n");
            break;
        }

        v->pos = (int)v->f_pos;
        /* bitshift to punch back the data to the Uint8 format ? */
        buffer[s++] += v->data[v->pos] * v->l_vol >> 8; /* s = +1 */
        buffer[s++] += v->data[v->pos] * v->r_vol >> 8; /* s = +2 */
        v->f_pos += v->pitch;
        printf("dong.\n");
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
    (*audio)->samples = 1024;
    (*audio)->callback = mixer;
    (*audio)->userdata = NULL;

    if(SDL_OpenAudio((*audio),NULL) != 0) {
        fprintf(stdout,"could not obtain audio... :<\n");
        exit(1);
    }

    /* from this point on data gets passed to the soundcard */
    SDL_PauseAudio(0);
}

void
load_audio(SDL_AudioSpec **audio, sound_t *sound)
{
    SDL_LoadWAV("default/file_complete.wav",(*audio),&sound->data,&sound->length);
}

void
free_audio(SDL_AudioSpec **audio) 
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free((*audio));
}

