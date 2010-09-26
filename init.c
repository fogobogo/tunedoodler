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
/* load images, set button to none */ 
void
init_ui(theme_t *ui, button_t *b)
{
    /* load images */
    ui->button = load_image(BUTTON);
    ui->icon = load_image(ICON);
    ui->play = load_image(PLAY);
    ui->playicon = load_image(PLAYICON);
	ui->stopicon = load_image(STOPICON);
    ui->line = load_image(LINE);

    /* set active button to none */
    b->active = BUTTON_NONE;
    b->before = BUTTON_NONE;
}

/* find out how much icons we have and how many buttons fit in the window */
void
init_total(SDL_Surface *display, theme_t *ui, button_t *b)
{
    int buttons, icons; /* buttons, icons */

    buttons = ((display->w - ui->play->w) / ui->button->w);
    /* calculate the number of icons. this assumes the icon width equals the icon height */
    icons = ui->icon->w / ui->icon->h; 

    /* if there are less icons than buttons that fit on the screen blit only as many buttons as there are icons */
    if(buttons > icons) { b->total = icons; }
    /* else blit as many buttons as possible which means there will be some icons hidden (or match the button number) */
    else { b->total = buttons; }
}

void
init_page(SDL_Surface *display, theme_t ui, page_t *pg)
{
	pg->num = 0;
	pg->before = pg->num;
	pg->width = display->w / ui.icon->h;
	pg->idx = pg->num * pg->width;
}


void
init_offsets(SDL_Surface *display, theme_t ui, metric_t *m, button_t b)
{
    /* store the values so we can later figure out what button was pressed */
    /* calculate the horizontal offset (left, right) relative to the window so the buttons are always centered on the screen */
    m->xoff = ((display->w - ui.play->w) - (ui.button->w * b.total)) / 2; /* from left */
    m->yoff = (display->h - (ui.icon->h * LINE_NO * 2)) / 2;
	/* needed for proper alignment */
	/* check if the result is odd / even and add a little offset to properly align the lines with the icons */
	if((m->yoff / ui.icon->h) % 2 == 0) {
		m->yoff += (ui.icon->h / 2);
	}
    printf("xoff: %d\n",m->xoff);
    printf("yoff: %d\n",m->yoff);
}

void
init_margins(theme_t ui, metric_t *m)
{
    /* to center icons on buttons */
    m->xm = (ui.button->w - ui.icon->h) / 2;
    m->ym = (ui.button->h - ui.icon->h) / 2;
}
