#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "define.h"
#include "config.h"
#include "sdl.c"
#include "init.c"
#include "pompcore.c"
#include "blit.c"
#include "update.c"
#include "audio.c"
#include "tune.c"

/* TODO: removal of dupes */
/* TODO: seperate ui from core code */
/* TODO: proper loading routine */
/* TODO: removal of nodes */
/* TODO: find a better way to organise the functions */
/* TODO: seperate event loop to a event.c */

/* TODO TODO TODO: ERROR CHECKING!!!!! */

int main (int argc, char *argv[])
{
    SDL_Surface *display;
    SDL_Surface *background;

    SDL_AudioSpec *audio;

    SDL_Event event;

    Uint32 frame; /* holds frame time */
    int loop = 1;
    int loop_tune = 0;
    int blit = 0;

    float pitch = 0.0;
    float vol = 1.0;


    int n = 0; /* node counter */

    sound_t sounds[1];

    pompface    ui; /* user interface resources. */
    point offset;
    point margin;


    tune_t *head;
    tune_t *cur;
    tune_t *new;


    SDL_Rect pos;
    SDL_Rect rel;
    SDL_Rect clip;

    atexit(SDL_Quit);

    /* let's go */
    display = init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT); /* init sdl */
    background = init_background(SCREEN_WIDTH,SCREEN_HEIGHT,BACKGROUND); /* init background */

    init_ui(&ui);
    init_tune(&head,&cur,&new);

    init_total(display,&ui);
    init_offsets(display,ui,&offset);
    init_margins(ui,&margin);

    init_audio(&audio);
    sounds[0].data = NULL;
    load_audio(&audio,&sounds[0]);
    /* SDL_LoadWAV("default/808-bassdrum.wav",audio,&sounds[0].data,&sounds[0].length); */
    if(sounds[0].data == NULL) {
        printf("ouch. no sound.\n");
    }


    SDL_Delay(300);
    process_audio(&voice[0],&sounds[0],2.0,2.0,3.0);
    if(voice[0].data == NULL) {
        printf("ouch.\n");
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
                        /* TODO get rid of this mess */
                        case SDLK_DOWN:
                            pitch -= 1.0;
                            process_audio(&voice[0],&sounds[0],vol,vol,pitch);
                            SDL_Delay(200);
                            break;
                        case SDLK_UP:
                            pitch += 1.0;
                            process_audio(&voice[0],&sounds[0],vol,vol,pitch);
                            SDL_Delay(200);
                            break;
                        case SDLK_LEFT:
                            update_display(background,display,pos);
                            pos.x -= ui.icon->h;
                            blit_click(display,ui,pos,clip);
                            break;
                        case SDLK_RIGHT:
                            update_display(background,display,pos);
                            pos.x += ui.icon->h;
                            blit_click(display,ui,pos,clip);
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

	}
    /*main loop ends here. */

    cur = head;
    while(cur->next != NULL) {
        printf("x: %d\ty: %d\ti: %d\n",cur->x,cur->y,cur->i);
        cur = cur->next;
    }

    printf("cleaning up... ");
    free_tune(&head,&cur);
    free_audio(&audio);
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
    SDL_FreeWAV(sounds[0].data);

    SDL_Quit();
    printf("done.\n");

    return(0);

}
