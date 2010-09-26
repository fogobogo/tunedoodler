/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/* voice_t voice[] is global */

/* dummy callback. */
void
dummy(void *data, Uint8 *stream, int len) 
{
}

void
process_audio(voice_t *voice, sound_t *sound, int vol, float pitch)
{
    voice->data = NULL; /* clear channel */

    /* process audio data */
    voice->len = sound->len / 2; /* why ?! stereo maybe ? */
    voice->pos = 0;
    voice->f_pos = 0.0;
	/* pump up the volume! pump it up! pump it up! */
	voice->vol = (int)vol * 256.0;
    voice->pitch = pitch;

	/* point the channel to the sound data */
    voice->data = (Sint16 *)sound->data;
}

void
mix_audio(void *data, Uint8 *stream, int len)
{
    int s;
    int i;
    Sint16 *buffer;

    /* buffer points to stream(?), why not just manipulate the stream directly ? */
	/* init buffer */
    buffer = (Sint16 *)stream;
	/* clearing the buffer, no idea why (artifacts?) */
	/* only the buffer gets played back? */
	memset(buffer, 0, len); /* not sure that is a good idea */

    len /= 4;

    /* for every channel (voice) ... */
    for(i=0; i < 10; ++i) {
		/* if there is no data start the next cycle */
		 if(!voice[i].data) { continue; }
			
		/* roll over the samples */
		for(s = 0; s < len; ++s) {
            if(voice[i].pos >= voice[i].len) {
                voice[i].data = NULL;
                break;
            }

			voice[i].pos = (Sint16)voice[i].f_pos; /* float to int */
			/* fill buffer, interleave audio signal */
            /* shift down by 10 to (hopefully) prevent clipping */
			buffer[s * 2] += voice[i].data[voice[i].pos] * voice[i].vol >> 10; /* left channel output */
			buffer[s * 2 + 1] += voice[i].data[voice[i].pos] * voice[i].vol >> 10; /* right channel output */
			voice[i].f_pos += voice[i].pitch;
		}
    }
}

void
init_audio()
{
    SDL_AudioSpec req;

    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stdout,"could not initalize audio... :<\n");
    }

    req.freq = 48000;
    req.format = AUDIO_S16SYS;
    req.channels = 2; /* stereo */
    req.samples = 4096; /* big buffer would be ok, we won't need a quick response */
    req.callback = mix_audio;
    req.userdata = NULL;

    if(SDL_OpenAudio(&req, &audio) != 0) {
        fprintf(stdout,"could not obtain audio... :<\n");
        exit(1);
    }

    /* from this point on data gets passed to the soundcard */
    SDL_PauseAudio(0);
}

void
load_audio(sound_t *sound)
{
	FILE *fd;
	int n = 0;
	char c[256];

	fd = fopen("default/sounds","r");

    SDL_LoadWAV("default/pluck2.wav",&audio,&sound[0].data,&sound[0].len);
    SDL_LoadWAV("default/tom2.wav",&audio,&sound[1].data,&sound[1].len);
    SDL_LoadWAV("default/conga2.wav",&audio,&sound[2].data,&sound[2].len);
    SDL_LoadWAV("default/distkick2.wav",&audio,&sound[3].data,&sound[3].len);

	fclose(fd);
}

void
free_audio() 
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
}

