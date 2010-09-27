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

typedef struct {
    Uint8 *data;
    Uint32 len;
} sound_t;

typedef struct {
    Sint16 *data;
    Uint32 len;
    int pos;
    float f_pos; /* pos casted as float */
	int vol;
    float pitch;
} voice_t;

/* playback channels. global variable */
voice_t voice[10 + 1];

SDL_AudioSpec audio;

