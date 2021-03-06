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
blit_buttons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b)
{
    SDL_Rect pos;
    int i;
    
	/* blit "play" button */
    SDL_BlitSurface(ui.play,NULL,surface,NULL);

    /* blit instrument buttons */
    for(i=0;i<b.total;i++) {
        pos.x = m.xoff + (ui.button->w * i);
        pos.y = 0;
        SDL_BlitSurface(ui.button,NULL,surface,&pos);
    }
}

void
blit_play(SDL_Surface *surface, theme_t ui)
{
	SDL_Rect pos;

	/* blit icon in the middle of the "play" button */
	pos.x = (ui.play->w - ui.playicon->w) / 2;
	pos.y = (ui.play->h - ui.playicon->h) / 2;
	pos.w = ui.playicon->w;
	pos.h = ui.playicon->h;
	SDL_BlitSurface(ui.playicon,NULL,surface,&pos);
}

void
blit_icons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b)
{
    SDL_Rect pos;
    SDL_Rect clip;
    int i; /* iterator */

    /* blit icons */

    for(i=0;i<b.total;i++) {
        /* clip icons */
        clip.x = ui.icon->h * i;
        clip.y = 0;
        clip.w = ui.icon->h;
        clip.h = ui.icon->h;

        /* position icons */
        pos.x = m.xoff + m.xm + ( (ui.icon->h + (m.xm * 2)) * i );
        pos.y = m.ym; 
        /* blit icon on surface */
        SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    }

}

void
blit_lines(SDL_Surface *surface, theme_t ui, metric_t m)
{
    SDL_Rect pos;
    int i; /* iterator */
    int x; /* x position */
   
    /* blit lines */

    for(i=0;i<LINE_NO;i++) {
        for(x=0;x<surface->w;x+=ui.line->w) {
            pos.x = x;
            pos.y = (m.yoff + ui.icon->h) + (ui.icon->h * i * 2);

            SDL_BlitSurface(ui.line,NULL,surface,&pos);
        }
    }
}

/* TODO: rename */
void
blit_click(SDL_Surface *surface, theme_t ui, SDL_Rect pos, SDL_Rect clip)
{
    SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    SDL_UpdateRect(surface,pos.x,pos.y,pos.w,pos.h);
}

/* blit the icon on the currrent position */
/* TODO: is that even used??? */
void
blit_cursor(SDL_Surface *display, SDL_Rect pos, SDL_Rect clip, theme_t ui)
{
    SDL_BlitSurface(ui.icon,&clip,display,&pos);
    SDL_UpdateRect(display,pos.x,pos.y,pos.w,pos.h);
}

