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
