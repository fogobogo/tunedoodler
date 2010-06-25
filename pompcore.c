#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>


void
init_num(SDL_Surface *surface, face *ui)
{
    int b, i; /* buttons, icons */
    int p; /* pixels */

    b = ((surface->w - ui->play->w) / ui->button->w);
    /* calculate the number of icons. this assumes the icon width equals the icon height */
    i = ui->icon->w / ui->icon->h; 

    /* if there are less icons than buttons that fit on the screen blit only as many buttons as there are icons */
    if(b > i) { ui->num = i; }
    /* else blit as many buttons as possible which means there will be some icons hidden (or match the button number) */
    else { ui->num = b; }
}

void
init_offsets(SDL_Surface *surface, face *ui)
{
    /* calculate the horizontal offset (left, right) relative to the window so the buttons are always centered on the screen */
    /* store the values so we can later figure out what button was pressed */
    ui->xoff = ((surface->w - ui->play->w) - (ui->button->w * ui->num)) / 2; /* from left */
    ui->yoff = (surface->h - (ui->line->h * LINE_NO * 2)) / 2;
    printf("xoff: %d\n",ui->xoff);
    printf("yoff: %d\n",ui->yoff);
}

void
blit_ui(SDL_Surface *surface, face ui)
{
    SDL_Rect pos;
    SDL_Rect clip;
    int i; /* iterator */
    int xm, ym; /* x, y margin */

    /* blit "play" button */
    SDL_BlitSurface(ui.play,NULL,surface,NULL);
    /* blit sound buttons */
    for(i=0;i<ui.num;i++) {
        pos.x = ui.xoff + (ui.button->w * i);
        pos.y = 0;
        SDL_BlitSurface(ui.button,NULL,surface,&pos);
    }
    /* blit icons */
    /* to center icons on buttons */
    xm = (ui.button->w - ui.icon->h) / 2;
    ym = (ui.button->h - ui.icon->h) / 2;

    for(i=0;i<ui.num;i++) {
        /* clip icons */
        clip.x = ui.icon->h * i;
        clip.y = 0;
        clip.w = ui.icon->h;
        clip.h = ui.icon->h;

        /* position icons */
        pos.x = ui.xoff + xm + ( (ui.icon->h + (xm * 2)) * i );
        pos.y = ym; 
        /* blit icon on surface */
        SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    }

}

void
blit_lines(SDL_Surface *background, face ui)
{
    SDL_Rect pos;
    int i; /* iterator */
    int x; /* x position */
   
    /* blit lines */
    for(i=0;i<LINE_NO;i++) {
        for(x=0;x<background->w;x+=ui.line->w) {
            pos.x = x;
            pos.y = (ui.yoff + ui.line->h) + (ui.line->h * i * 2) + (ui.icon->h / 2);

            SDL_BlitSurface(ui.line,NULL,background,&pos);
        }
    }
}

/* create background with color */
SDL_Surface*
init_background(unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b)
{
    SDL_Surface *tmp;
    tmp = SDL_CreateRGBSurface(SDL_FLAGS,w,h,DEPTH,0,0,0,0);
    SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,r,b,g));
    return(tmp);
}

int
button_click(SDL_Surface *surface, SDL_Event event, face ui)
{
    int tmp;
    if( event.button.x > ui.xoff && 
        event.button.y < ui.button->h && 
        event.button.x < ui.button->w * ui.num + ui.xoff) {
        tmp = (event.button.x - ui.xoff) / ui.button->w;
        return(tmp);
    }

    else { return(ui.active); }

}

void
update_clip(face ui, SDL_Rect *clip)
{
    if(ui.active > -1) {
        clip->x = ui.icon->h * ui.active;
        clip->y = 0;
        clip->w = ui.icon->h;
        clip->h = ui.icon->h;
    }
}

void
update_pos(SDL_Event event, face ui, SDL_Rect *pos)
{
    int x, y;
    if(ui.active > -1) {

        if(event.motion.y > ui.yoff) {
            pos->x = (event.motion.x / ui.icon->h) * ui.icon->h;
            pos->y = (event.motion.y / ui.icon->h) * ui.icon->h;
            pos->w = ui.icon->h;
            pos->h = ui.icon->h;
        }

        else {
            pos->x = event.motion.x;
            pos->y = event.motion.y;
            pos->w = ui.icon->h;
            pos->h = ui.icon->h;
        }
    }
}

void
draw_cursor(SDL_Surface *surface, SDL_Rect pos, face ui)
{
    SDL_Rect clip;

    clip.x = ui.icon->h * ui.active;
    clip.y = 0;
    clip.w = ui.icon->h;
    clip.h = ui.icon->h;

    SDL_BlitSurface(ui.icon,&clip,surface,&pos);
    SDL_UpdateRect(surface,pos.x,pos.y,pos.w,pos.h);
}
