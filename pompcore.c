#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


void
blit_buttons(SDL_Surface *surface, pompface ui, point offset)
{
    SDL_Rect pos;
    int i;

    /* blit "play" button */
    SDL_BlitSurface(ui.play,NULL,surface,NULL);
    /* blit instrument buttons */
    for(i=0;i<ui.total;i++) {
        pos.x = offset.x + (ui.button->w * i);
        pos.y = 0;
        SDL_BlitSurface(ui.button,NULL,surface,&pos);
    }
}

void
blit_icons(SDL_Surface *surface, pompface ui, point offset, point margin)
{
    SDL_Rect pos;
    SDL_Rect clip;
    int i; /* iterator */

    /* blit icons */

    for(i=0;i<ui.total;i++) {
        /* clip icons */
        clip.x = ui.icon->h * i;
        clip.y = 0;
        clip.w = ui.icon->h;
        clip.h = ui.icon->h;

        /* position icons */
        pos.x = offset.x + margin.x + ( (ui.icon->h + (margin.x * 2)) * i );
        pos.y = margin.y; 
        /* blit icon on surface */
        SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    }

}

void
blit_lines(SDL_Surface *surface, pompface ui, point offset)
{
    SDL_Rect pos;
    int i; /* iterator */
    int x; /* x position */
   
    /* blit lines */

    for(i=0;i<LINE_NO;i++) {
        for(x=0;x<surface->w;x+=ui.line->w) {
            pos.x = x;
            pos.y = (offset.y + ui.icon->h) + (ui.icon->h * i * 2);

            SDL_BlitSurface(ui.line,NULL,surface,&pos);
        }
    }
}

/* kind of bad. consider replacing it with an SDL_Rect array */
void
button_click(SDL_Event event, SDL_Surface *display, pompface *ui, point offset)
{
    if( event.button.x > offset.x && 
        event.button.y < ui->button->h && 
        event.button.x < ui->button->w * ui->total + offset.x) {
        ui->before = ui->active;
        ui->active = (event.button.x - offset.x) / ui->button->w;
    }

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

/* blit the icon on the currrent position */
void
blit_cursor(SDL_Surface *display, SDL_Rect pos, SDL_Rect clip, pompface ui)
{
    SDL_BlitSurface(ui.icon,&clip,display,&pos);
    SDL_UpdateRect(display,pos.x,pos.y,pos.w,pos.h);
}

void
update_display(SDL_Surface *surface, SDL_Surface *display, SDL_Rect pos)
{
    SDL_BlitSurface(surface,&pos,display,&pos);
    SDL_UpdateRect(display,pos.x,pos.y,pos.w,pos.h);
}

int
detect_motion(SDL_Event event, pompface ui, SDL_Rect pos)
{
    /* check if the pointer moved to another cell of the grid */
    if(((event.motion.x / ui.icon->h) * ui.icon->h != pos.x
    && ui.active > BUTTON_NONE)
    || ((event.motion.y / ui.icon->h) * ui.icon->h != pos.y
    && ui.active > BUTTON_NONE)) {
        return(MOTION);
    }
}

void
init_audio()
{
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,512) == 1) {
        fprintf(stderr,"%s\n",Mix_GetError());
    }

    else { Mix_Volume(-1,MIX_MAX_VOLUME); } /* set volume to max */
}

int
check_bounds(SDL_Event event, SDL_Surface *display, pompface ui, point offset)
{
    if(((event.button.y > offset.y) 
       && (ui.active >  BUTTON_NONE))
       || ((event.button.y > display->h - offset.y) 
       && (ui.active >  BUTTON_NONE))) {

        return(OK);
    }
}

void
blit_click(SDL_Surface *surface, pompface ui, SDL_Rect pos, SDL_Rect clip)
{
    SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    SDL_UpdateRect(surface,pos.x,pos.y,pos.w,pos.h);
}

void
play_tune(Mix_Chunk *son[][2], int tempo, tune **cur, tune **head, int n)
{
    int time; 
    /* start at the beginning */
    time = 0;
    (*cur) = (*head);

    while(n != 0) {
        while((*cur)->x == time) {
            Mix_PlayChannel(-1,son[(*cur)->y][(*cur)->i],0);
            (*cur) = (*cur)->next;
            n--;
            printf("n: %d\n",n);
        }

        SDL_Delay(tempo);
        time++;
    }
}

void
update_rel(SDL_Event event, pompface ui, point offset, SDL_Rect *rel)
{
    rel->x = event.button.x / ui.icon->h;
    rel->y = (event.button.y - offset.y) / ui.icon->h;
}
