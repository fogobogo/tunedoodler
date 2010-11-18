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
#include "SDL/SDL_mixer.h"

Mix_Chunk *sound[4];

/* dummy callback. */
void
dummy(void *data, Uint8 *stream, int len) 
{
}

void 
pitch_audio(int chan, void *stream, int len, void *udata)
{
    int sample;
    Sint16 *data;
    Sint16 *copy;
    int pos = 0;
    int s;

    data = (Sint16 *)stream;

    len /= 4;

    for(sample = 0; sample < len / 2; sample++) {
        /* simple playback
        data[sample * 2] = (data[sample * 2] * 256) >> 8;
        data[sample * 2 + 1] = (data[sample * 2 + 1] * 256) >> 8;
        */

        /* pitch */
        /* jitters like hell. 1.0 is fine though */
        data[sample * 2] = data[pos * 2] * 256 >> 8;
        data[sample * 2 + 1] = data[pos * 2 + 1] * 256 >> 8;
        pos += 2;
    }

}

void
init_audio()
{
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stdout,"could not initalize audio... :<\n");
    }

    if(Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048) == 1) {
        printf("%s\n",Mix_GetError());
    }

    Mix_Volume(-1, MIX_MAX_VOLUME);
    
    if(Mix_RegisterEffect(MIX_CHANNEL_POST, pitch_audio, NULL, NULL) == 0) {
        printf("error.");
    }
}

void
load_audio()
{
    sound[0] = Mix_LoadWAV("default/test.wav");
    sound[1] = Mix_LoadWAV("default/tom2.wav");
    sound[2] = Mix_LoadWAV("default/conga2.wav");
    sound[3] = Mix_LoadWAV("default/distkick2.wav");
}

void
play(tune_t *cur, tune_t *head, int tempo)
{
    int x; /* position */

    float pitch[10 + 1];

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

    while(cur->next != NULL) { 
        while(cur->x == x) {
            Mix_PlayChannel(-1,sound[cur->i],0);
            cur = cur->next;
        } 

        x++;
        SDL_Delay(tempo);
    }
    printf("time: %dms\n", x * tempo);
}

void
free_audio()
{
        Mix_FreeChunk(sound[0]);
        Mix_FreeChunk(sound[1]);
        Mix_FreeChunk(sound[2]);
        Mix_FreeChunk(sound[3]);
        Mix_CloseAudio();
}

