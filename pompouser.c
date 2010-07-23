#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "define.h"
#include "config.h"
#include "sdl.c"
#include "init.c"
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
    int loop_tune = 0;
    int blit = 0;

    int click;

    int n = 0; /* node counter */
    int i,o = 0; /* general purpose iterator */

    char *instrumenta[] = { INSTRUMENTA, NULL };
    char *instrumentb[] = { INSTRUMENTB, NULL };
    Mix_Chunk *son[10][2];

    pompface    ui; /* user interface resources. */
    point offset;
    point margin;


    tune *head;
    tune *cur;
    tune *new;


    SDL_Rect pos;
    SDL_Rect rel;
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
        son[i][0] = Mix_LoadWAV(instrumenta[i]);
        son[i][1] = Mix_LoadWAV(instrumentb[i]);
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
                        case SDLK_DOWN:
                            printf("down\n");
                            printf("pos x: %d y: %d\n",pos.x, pos.y);
                            pos.x += ui.icon->h;
                            printf("pos x: %d y: %d\n",pos.x, pos.y);
                            blit_click(background,ui,pos,clip);
                            break;
						case SDLK_q:
							loop = 0;
							break;
                        case SDLK_p:
                            /* set the active button to none */
                            ui.active = BUTTON_NONE;
                            printf("ui.active: %d\n",ui.active);
                            /* trigger a blit to get it actually disappear on the screen */
                            update_display(background,display,pos);
                            /* sort all nodes so they can be played in order */
                            head = msort_tune(head,n);
                            /* play them! */
                            play_tune(son,TEMPO,&cur,&head,n);
                            printf("n: %d\n",n);
                            break;
                        case SDLK_s:
                            head = msort_tune(head,n);
                            cur = head;
                            print_tune(&head,&cur);
                            break;
                        case SDLK_l:
                            if(loop_tune == 0) {
                                loop_tune = 1;
                            }
                            else { loop_tune = 0; }
                            ui.active = BUTTON_NONE;
                            update_display(background,display,pos);
                            head = msort_tune(head,n);
                            printf("loop: %d\n",loop_tune);
                            break;
                        case SDLK_F5:
                            head = msort_tune(head,n);
                            save_tune(&head,&cur);
                            break;
                        default:
                            break;
					}
                    break; /* prevent falling through */

                /* mouse button handling */
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            /* find out if a button was clicked and set the active button to it */
                            button_click(event,display,&ui,offset);
                            /* update the clipping rectangle for the icon. but only if the active button isn't none */
                            update_clip(ui,&clip);
                            /* check if the current positon is in the snap grid zone */
                            if(check_bounds(event,display,ui,offset) == OK) {
                                update_pos(event,ui,offset,&pos);
                                update_rel(event,ui,offset,&rel);
                                /* initalize the members of the current node with the relative values */
                                store_tune(&cur, rel.x, rel.y, ui.active);
                                /* create the next node */
                                create_tune(&cur,&new);
                                n++;
                                /* make the button click actually appear on screen */
                                blit_click(background,ui,pos,clip);
                            }
                            printf("#: %d\n",ui.active);
                            printf("nodes: %d\n",n);
                            break;
                        case SDL_BUTTON_RIGHT:
                            /* set button to none on right click */
                            ui.active = BUTTON_NONE;
                            update_display(background,display,pos);
                            delete_tune(rel.x, rel.y, &n,&head,&cur);
                            printf("#: %d\n",ui.active);
                            break;
                        default:
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
                    /* TODO: handle resizes */
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

        if(loop_tune == 1) {
            play_tune(son,TEMPO,&cur,&head,n);
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
    for(o=0;o<2;o++) {
    for(i=0;i<10;i++) {
       Mix_FreeChunk(son[i][o]); 
    }
    }

    Mix_CloseAudio();
    SDL_Quit();
    printf("done.\n");

    return(0);

}
