#include <stdio.h>
#include <SDL/SDL.h>

#include "config.h"

SDL_Surface* 
init_sdl(unsigned int screen_width, unsigned int screen_height) 
{
	SDL_Surface *tmp;
	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT) == -1) {
		fprintf(stdout,"%s\n",SDL_GetError());
		exit(1);
	}

	else {
		tmp = SDL_SetVideoMode(screen_width,screen_height,DEPTH,SDL_FLAGS);
		if(tmp == NULL) {
			fprintf(stdout,"%s\n",SDL_GetError());
			exit(1);
		}
		else
            SDL_WM_SetCaption(TITLE,NULL);
			/* nice leak here. */
			return(tmp);
	}
}


SDL_Surface* 
load_image(const char *filename) {
	SDL_Surface *tmp;
	SDL_Surface *surface;

	tmp = SDL_LoadBMP(filename);
	if(tmp == NULL) {
		fprintf(stdout,"%s\n",SDL_GetError());
		SDL_FreeSurface(tmp);
		exit(1);
	}

	surface = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	if(surface == NULL) {
		fprintf(stdout,"%s\n",SDL_GetError());
		SDL_FreeSurface(surface);
		exit(1);
	}

	SDL_SetColorKey(surface,SDL_SRCCOLORKEY,SDL_MapRGB(surface->format,TRANSPARENT));

	return(surface);
}

/* set the whole surface to a defined colour */
void 
set_colour(SDL_Surface *dst, int r, int g, int b) {
	Uint32 color;
	color = SDL_MapRGB(dst->format,r,g,b);
	SDL_FillRect(dst,NULL,color);
}

