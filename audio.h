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
void dummy(void *data, Uint8 *stream, int len);
void process_audio(voice_t *voice, sound_t *sounds, int vol, float pitch);
void mix_audio(void *data, Uint8 *stream, int length);
void init_audio(SDL_AudioSpec **audio);
void load_audio(SDL_AudioSpec **audio, sound_t *sound)
void free_audio(SDL_AudioSpec **audio) 
