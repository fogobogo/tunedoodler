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

void
update_display(SDL_Surface *surface, SDL_Surface *display, SDL_Rect pos)
{
    SDL_BlitSurface(surface, &pos, display, &pos);
    SDL_UpdateRect(display, pos.x, pos.y, pos.w, pos.h);
}

/* update the clipping rectangle for the icons */
void
update_clip(theme_t ui, int active, SDL_Rect *clip)
{
    /* update the clipping rectangle for the icons */
    if(active > BUTTON_NONE) {
        clip->x = ui.icon->h * active;
        clip->y = 0;
        clip->w = ui.icon->h;
        clip->h = ui.icon->h;
    }
}

/* get the current absolute position */
void
update_pos(SDL_Event event, theme_t ui, metric_t m, SDL_Rect *pos)
{
    /* update the current position of the mouse cursor */
	/* TODO: make resolution independent */
    if(event.motion.y > m.yoff && event.motion.y < 480 - 32) { /* if the cursor is in the snap grid zone */
		/* snap to grid */
        pos->x = (event.motion.x / ui.icon->h) * ui.icon->h;
        pos->y = (event.motion.y / ui.icon->h) * ui.icon->h;
        pos->w = ui.icon->h;
        pos->h = ui.icon->h;
    }

    else { /* otherwise current mouse position = current mouse position */
        pos->x = event.motion.x;
        pos->y = event.motion.y;
        pos->w = ui.icon->h;
        pos->h = ui.icon->h;
    }
}

/* get the relative position */
/* TODO: needs to integrate with the current page */
void
update_rel(SDL_Event event, theme_t ui, metric_t m, SDL_Rect *rel)
{
    rel->x = event.button.x / ui.icon->h;
    rel->y = (event.button.y - m.yoff) / ui.icon->h;
}

