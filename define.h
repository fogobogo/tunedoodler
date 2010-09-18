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

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	480
#define DEPTH	32
#define SDL_FLAGS	SDL_SWSURFACE|SDL_RESIZABLE
#define SDL_INIT    SDL_INIT_VIDEO|SDL_INIT_AUDIO


#define TITLE "tUnEdoODle"

#define BUTTON_NONE     -1
#define MOTION          1
#define NO_MOTION       0
#define CLICK           1
#define OK              1
#define FAIL            0
#define NOTHING			0

/* interface struct. holds pointers to the user interface images and their metrics */
typedef struct {
    SDL_Surface *line;
    SDL_Surface *button;
    SDL_Surface *button_pressed;
    SDL_Surface *icon;
    /* SDL_Surface *icon_pressed */
    SDL_Surface *play;
    /* SDL_Surface *play_pressed; */
} theme_t;

/* metrics struct. holds values of various offsets to place user interface elements at the right spot on the screen */
typedef struct {
    int xoff;  /* horizontal offset */
    int yoff;  /* vertical offset */
    int xm;    /* horizontal margin */
    int ym;    /* vertical margin */
} metric_t;

/* button struct to keep track of the various button states */
typedef struct {
    int active;         /* active button */
    int before; 
    int total;          /* total number of buttons */
} button_t;

typedef struct {
	int num;
	int before;
	int width;
	int idx;
} page_t;

typedef struct tune_t {
    unsigned int x;
    unsigned int y;
    int i;
    struct tune_t *next;
} tune_t;


typedef struct {
    Uint8 *data;
    Uint32 length;
} sound_t;


typedef struct {
    Sint16 *data;
    unsigned int length;
    int pos;
    float f_pos; /* pos casted as float */
	int vol;
    float pitch;
} voice_t;

/* playback channels. global variable */
voice_t voice[10];
