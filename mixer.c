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
