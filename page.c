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
clear_page(SDL_Surface *restore, SDL_Surface *background, SDL_Surface *display)
{
	SDL_BlitSurface(restore,NULL,background,NULL);
	SDL_BlitSurface(background,NULL,display,NULL);
	SDL_UpdateRect(display,0,0,0,0);
	printf("cleared page.\n");
}

void
update_page_pos(int x, int y, theme_t ui, metric_t m, page_t pg, SDL_Rect *pos)
{
	pos->x = (x - pg.idx) * ui.icon->h;
	pos->y = y * ui.icon->h + m.yoff;
	pos->w = ui.icon->h;
	pos->h = ui.icon->h;
}

void
update_page_clip(int i, theme_t ui, SDL_Rect *clip)
{
	clip->x = ui.icon->h * i;
	clip->y = 0;
	clip->w = ui.icon->h;
	clip->h = ui.icon->h;
}
