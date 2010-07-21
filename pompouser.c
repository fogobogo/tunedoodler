#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "define.h"
#include "config.h"
#include "sdl.c"
#include "init.c"
#include "node.c"
#include "pompcore.c"
#include "tune.c"

/* TODO: removal of dupes */
/* TODO: seperate ui from core code */
/* TODO: proper loading routine */
/* TODO: removal of nodes */

int main (int argc, char *argv[])
{
    SDL_Surface *display;
    SDL_Surface *background;
    SDL_Surface *lines;
    SDL_Surface *overlay;

    SDL_Event event;

    Uint32 frame; /* holds frame time */
    int loop = 1;
    int blit = 0;

    int click;

    int n = 0; /* node counter */
    int i = 0; /* general purpose iterator */

    char *instrument[] = { INSTRUMENTA, NULL };
    Mix_Chunk *son[10];

    pompface    ui; /* user interface resources. */
    point offset;
    point margin;


    tune *head;
    tune *cur;
    tune *new;


    SDL_Rect pos;
    SDL_Rect clip;

    atexit(SDL_Quit);
    atexit(Mix_CloseAudio);

    /* let's go */
    display = init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT); /* init sdl */
    background = init_background(SCREEN_WIDTH,SCREEN_HEIGHT,BACKGROUND); /* init background */

    init_ui(&ui);
    init_tune(&head,&cur,&new);

    init_total(display,&ui);
    init_offsets(display,ui,&offset);
    init_margins(ui,&margin);

    init_audio();
    for(i=0;i<10;i++) {
        son[i] = Mix_LoadWAV(instrument[i]);
    }

    blit_lines(background,ui,offset);
    blit_buttons(background,ui,offset);
    blit_icons(background,ui,offset,margin);

    SDL_BlitSurface(background,NULL,display,NULL);
    SDL_UpdateRect(display,0,0,0,0);
   
    /* MAIN LOOP */
    while(loop) {
		/* get ticks for FPS calculation */
		frame = SDL_GetTicks();

		/* event loop */
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				/* keyboard handling */
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_q:
							loop = 0;
							break;
                        case SDLK_p:
                            ui.active = BUTTON_NONE;
                            update_pos(event,ui,offset,&pos);
                            update_display(background,display,pos);
                            head = msort_tune(head,n);
                            play_tune(son,TEMPO,&cur,&head,n);
                            printf("n: %d\n",n);
                            break;
                        case SDLK_s:
                            head = msort_tune(head,n);
                            cur = head;
                            print_tune(&head,&cur);
                            break;
					}
                /* mouse button handling */
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            button_click(event,display,&ui,offset);
                            update_clip(ui,&clip);
                            if(check_bounds(event,display,ui,offset) == OK) {
                                store_tune(event,display,ui,offset,&cur,&new);
                                n++;
                                update_pos(event,ui,offset,&pos);
                                blit_click(background,ui,pos,clip);
                            }
                            printf("#: %d\n",ui.active);
                            printf("nodes: %d\n",n);
                            break;
                        case SDL_BUTTON_RIGHT:
                            /* set button to none on right click */
                            ui.active = BUTTON_NONE;
                            update_display(background,display,pos);
                            update_pos(event,ui,offset,&pos);
                            delete_tune(&n,&head,&cur,offset,pos);
                            printf("#: %d\n",ui.active);
                            break;
                    }
                /* mouse motion handling */
                case SDL_MOUSEMOTION:
                    if(detect_motion(event,ui,pos) == 1) {
                        update_display(background,display,pos);
                        printf("%d\n",blit++);
                        update_pos(event,ui,offset,&pos);
                        blit_cursor(display,pos,clip,ui);
                    }
                    break;

                default:
                    /* prevent falling through */
                    break;

				/* thats the "x" on the window */
				case SDL_QUIT:
					loop = 0;
					break;

                case SDL_VIDEORESIZE:
                    printf("todo: resize");
                    break;
			}

		/* event loop ends here */
		}
       
        /*
        SDL_UpdateRect(display,0,0,0,0);
        */

		/* limit framerate */
		frame = SDL_GetTicks() - frame;
		if(frame < 1000/FRAMERATE) {
			SDL_Delay((1000/FRAMERATE) - frame);
		}


	}
    /*main loop ends here. */

    cur = head;
    while(cur->next != NULL) {
        printf("x: %d\ty: %d\ti: %d\n",cur->x,cur->y,cur->i);
        cur = cur->next;
    }

    printf("cleaning up... ");
    free_tune(&head,&cur);
    /* offload ui */
    SDL_FreeSurface(ui.line);
    SDL_FreeSurface(ui.button);
    SDL_FreeSurface(ui.button_pressed);
    SDL_FreeSurface(ui.icon);
    /* SDL_FreeSurface(ui.icon_pressed); */

    /* offload main surfaces */
    SDL_FreeSurface(background); 
    /*
    SDL_FreeSurface(lines); 
    SDL_FreeSurface(overlay); 
    */
    SDL_FreeSurface(display);
    /* offload instrument samples */
    for(i=0;i<10;i++) {
       Mix_FreeChunk(son[i]); 
    }

    Mix_CloseAudio();
    SDL_Quit();
    printf("done.\n");

    return(0);

}
