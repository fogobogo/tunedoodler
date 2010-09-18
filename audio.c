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

#include "audio.h"

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
		/* multiply length by two because... length has been halfed before??? */
		/* segfaults */
		/* length = length * 2; */
		for(s = 0; s < length; ++s) {
			/* WHAT DOES THIS DO ?? */
			if(v->pos >= v->length) {
				v->data = NULL;
				/* printf("ding.\n"); */
				break;
			}

			v->pos = (Sint16)v->f_pos; /* float to int */
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
	FILE *fd;
	int n = 0;
	char c[256];

	fd = fopen("default/sounds","r");

    SDL_LoadWAV("default/bass.wav",(*audio),&sound[0].data,&sound[0].length);
    SDL_LoadWAV("default/cowbell.wav",(*audio),&sound[1].data,&sound[1].length);
    SDL_LoadWAV("default/ding.wav",(*audio),&sound[2].data,&sound[2].length);
    SDL_LoadWAV("default/flutter.wav",(*audio),&sound[3].data,&sound[3].length);

	fclose(fd);
}

void
free_audio(SDL_AudioSpec **audio) 
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    free((*audio));
}

