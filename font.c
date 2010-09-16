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
init_font(SDL_Surface **font)
{
	(*font) = load_image(FONT);
}

void
do_print(char *str, int xpos, int ypos, SDL_Surface *font, SDL_Surface *surface)
{
	int i = 0; /* iterator */
	int col; /* number of chars per line */

	int x, y;

	SDL_Rect character;
	SDL_Rect pos;

	col = font->w / FONT_WIDTH;

	while(str[i] != '\0') {
		y = str[i] / col;
		x = str[i] - (y * col); 
		character.x = FONT_WIDTH * x; 
		character.y = FONT_HEIGHT * y;
		character.w = FONT_WIDTH;
		character.h = FONT_HEIGHT;

		pos.x = i * FONT_WIDTH + xpos;
		pos.y = ypos;
		pos.w = FONT_WIDTH;
		pos.h = FONT_HEIGHT;

		SDL_BlitSurface(font,&character,surface,&pos);
		i++;
	}
	
	SDL_UpdateRect(surface,(pos.x - (FONT_WIDTH * i)),pos.y,(pos.w * i),pos.h);
}

