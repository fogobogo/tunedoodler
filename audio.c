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
#include "audio.h"

/* dummy callback. */
void
dummy(void *data, Uint8 *stream, int len) 
{
}

void
process_audio(int y, int i, int vol, float pitch)
{
    voice[y].data = NULL; /* stop channel */

    /* process audio data */
    /* The data that comes in stream is a pointer of size len bytes, but your data is 16 bit (2 bytes) then the size will be len/2. */
    voice[y].len = sound[i].len / 2;
    voice[y].pos = 0;
    voice[y].f_pos = 0.0;
	/* pump up the volume! pump it up! pump it up! */
	voice[y].vol = (int)vol * 256.0;
    voice[y].pitch = pitch;

	/* point the channel to the sound data */
    voice[y].data = (Sint16 *)(sound[i].data);
}

/* that is the callback */
void
mix_audio(void *udata, Uint8 *stream, int len)
{
    int s;
    int y;
    Sint16 *data;
    voice_t *playback;

	/* init buffer */
    data = (Sint16 *)stream;
	/* memset(buffer, 0, len); */

    len /= 4;

    /* for every channel (voice) ... */
    for(y=0; y <= 10; y++) {

        playback = &voice[y];
        /* never gets executed? */
        if(!playback->data) { continue; printf("nol\n"); }
			
		/* roll over the samples */
		for(s = 0; s < len; s++) {
            if(playback->pos >= playback->len) {
                playback->data = NULL;
                break;
            }

			/* fill buffer, interleave audio signal */
            /* shift down by 10 to (hopefully) prevent clipping */
			playback->pos = (Sint16)playback->f_pos;
			data[s * 2] += playback->data[playback->pos] * playback->vol >> 10; /* left channel output */
			data[s * 2 + 1] += playback->data[playback->pos + 1] * playback->vol >> 10; /* right channel output */
			playback->f_pos += (float)playback->pitch;
		}
    }
}

void
init_audio()
{
    SDL_AudioSpec req; /* request */

    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stdout,"could not initalize audio... :<\n");
    }

    req.freq = 44100;
    req.format = AUDIO_S16SYS;
    req.channels = 2; /* stereo */
    req.samples = 1024; /* big buffer would be ok, we won't need a quick response */
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
load_audio()
{
    SDL_LoadWAV("default/pluck2.wav",&audio,&sound[0].data,&sound[0].len);
    SDL_LoadWAV("default/tom2.wav",&audio,&sound[1].data,&sound[1].len);
    SDL_LoadWAV("default/conga2.wav",&audio,&sound[2].data,&sound[2].len);
    SDL_LoadWAV("default/distkick2.wav",&audio,&sound[3].data,&sound[3].len);
}

void
play(tune_t *cur, tune_t *head, int tempo)
{
    int x; /* position */

    float pitch[10 + 1];

    /* TODO: replace that crap */
    pitch[11] = 1.0;
    pitch[10] = 1.25;
    pitch[9] = 1.5;
    pitch[8] = 1.75;
    pitch[7] = 2.0;
    pitch[6] = 2.25;
    pitch[5] = 2.5;
    pitch[4] = 2.75;
    pitch[3] = 3.0;
    pitch[2] = 3.25;
    pitch[1] = 3.5;
    pitch[0] = 3.75;


    /* start at the beginning */
    x = 0; 
    cur = head; /* dito */

    while(cur->next != NULL) { /* as long as we dont run out of nodes ... */
        while(cur->x == x) { /* ...and as long as cur->x matches x... */
            /* ...fill channel (voice) with the payload (sound) we want to pass to the output (soundcard)... */
            process_audio(cur->y, cur->i, 1, pitch[cur->y]);
            printf("playing sound %d\n on y: %d\n with pitch %.2f\n",cur->i,cur->y,pitch[cur->y]);
            printf("-- x: %d\n",x); 
            /* ...move on to next node... */ 
            cur = cur->next;
        } /* ...and when we are done... */

        SDL_Delay(tempo); /* .. give some time for playback .. */
        x++; /* ...and increase x */
    }
    printf("time: %dms\n", x * tempo);
}

void
free_audio()
{
    SDL_FreeWAV(sound[0].data);
    SDL_FreeWAV(sound[1].data);
    SDL_FreeWAV(sound[2].data);
    SDL_FreeWAV(sound[3].data);
    SDL_PauseAudio(1);
    SDL_CloseAudio();
}
