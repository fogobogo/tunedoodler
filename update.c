
void
update_display(SDL_Surface *surface, SDL_Surface *display, SDL_Rect pos)
{
    SDL_BlitSurface(surface,&pos,display,&pos);
    SDL_UpdateRect(display,pos.x,pos.y,pos.w,pos.h);
}

/* update the clipping rectangle for the icons */
void
update_clip(pompface ui, SDL_Rect *clip)
{
    /* update the clipping rectangle for the icons */
    if(ui.active > BUTTON_NONE) {
        clip->x = ui.icon->h * ui.active;
        clip->y = 0;
        clip->w = ui.icon->h;
        clip->h = ui.icon->h;
    }
}

/* get the current absolute position */
void
update_pos(SDL_Event event, pompface ui, point offset, SDL_Rect *pos)
{
    /* update the current position of the mouse cursor */
    if(event.motion.y > offset.y) { /* if the cursor is in the snap grid zone */
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
update_rel(SDL_Event event, pompface ui, point offset, SDL_Rect *rel)
{
    rel->x = event.button.x / ui.icon->h;
    rel->y = (event.button.y - offset.y) / ui.icon->h;
}

