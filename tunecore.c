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
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>



/* kind of bad. consider replacing it with an SDL_Rect array */
void
button_click(SDL_Event event, theme_t ui, metric_t m, button_t *b)
{
    if( event.button.x > m.xoff && 
        event.button.y < ui.button->h && 
        event.button.x < ui.button->w * b->total + m.xoff) {
        b->before = b->active;
        b->active = (event.button.x - m.xoff) / ui.button->w;
    }

}


/* check if the pointer (the mouse) moved to another cell of the grid */
int
detect_motion(SDL_Event event, theme_t ui, button_t b, SDL_Rect pos)
{
    if(((event.motion.x / ui.icon->h) * ui.icon->h != pos.x
    && b.active > BUTTON_NONE)
    || ((event.motion.y / ui.icon->h) * ui.icon->h != pos.y
    && b.active > BUTTON_NONE)) {
        return(MOTION);
    }

    else { return(NO_MOTION); }
}


int
check_bounds(SDL_Event event, SDL_Surface *display, theme_t ui, metric_t m, button_t b)
{
    if(((event.button.y > m.yoff) 
       && (b.active >  BUTTON_NONE))
       || ((event.button.y > display->h - m.yoff) 
       && (b.active >  BUTTON_NONE))) {

        return(1);
    }

	else { return(0); }
}

int
check_play(SDL_Event event, theme_t ui)
{
	if(event.button.y < ui.play->h
		&& event.button.x < ui.play->w) {

		return(1);
	}

    else { return(0); }
}


