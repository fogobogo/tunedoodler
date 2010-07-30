#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>



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


/* check if the pointer (the mouse) moved to another cell of the grid */
int
detect_motion(SDL_Event event, pompface ui, SDL_Rect pos)
{
    if(((event.motion.x / ui.icon->h) * ui.icon->h != pos.x
    && ui.active > BUTTON_NONE)
    || ((event.motion.y / ui.icon->h) * ui.icon->h != pos.y
    && ui.active > BUTTON_NONE)) {
        return(MOTION);
    }

    else { return(NO_MOTION); }
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
play_tune(Mix_Chunk *son[][2], int tempo, tune_t **cur, tune_t **head, int n)
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

