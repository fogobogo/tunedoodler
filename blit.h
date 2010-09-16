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

void blit_buttons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b);
void blit_icons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b);
void blit_lines(SDL_Surface *surface, theme_t ui, metric_t m);
void blit_click(SDL_Surface *surface, theme_t ui, SDL_Rect pos, SDL_Rect clip);
void blit_cursor(SDL_Surface *display, SDL_Rect pos, SDL_Rect clip, theme_t ui);
