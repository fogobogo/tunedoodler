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

SDL_Surface* init_background(unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b);
void init_ui(theme_t *ui, button_t *b);
void init_total(SDL_Surface *display, theme_t *ui, button_t *b);
void init_offsets(SDL_Surface *display, theme_t ui, metric_t *m, button_t b);
void init_margins(theme_t ui, metric_t *m);
