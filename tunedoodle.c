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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "define.h"
#include "config.h"
#include "sdl.c"
#include "audio.c"
#include "init.c"
#include "page.c"
#include "tunecore.c"
#include "blit.c"
#include "update.c"
#include "tune.c"
#include "font.c"

/* TODO: removal of dupes */
/* TODO: proper sound loading routine */
/* DONE: removal of nodes */
/* TODO: find a better way to organise the functions */
/* TODO: seperate event loop to a event.c */

/* TODO TODO TODO: ERROR CHECKING!!!!! */

int main (int argc, char *argv[])
{
    SDL_Surface *display;
    SDL_Surface *background;
	SDL_Surface *restore;
	SDL_Surface *font;

    SDL_AudioSpec *audio;

    SDL_Event event;

    Uint32 frame; /* holds frame time */
    int loop = 1;
    int loop_tune = 0;
    int blit = 0;

    float pitch = 0.05;
    int vol = 1;

	int i;
	unsigned int tempo;


    int n = 0; /* node counter */


    theme_t		ui; /* user interface resources. */
    metric_t    m; 	/* offsets, margins */
    button_t    b;  /* button tracking */
	page_t		pg; /* page tracking */

    tune_t *head;
    tune_t *cur;
    tune_t *new;

    sound_t sounds[4];

    SDL_Rect pos;
    SDL_Rect rel;
    SDL_Rect clip;

    atexit(SDL_Quit);

    /* let's go */
    display = init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT); /* init sdl */
    background = init_background(SCREEN_WIDTH,SCREEN_HEIGHT,BACKGROUND); /* init background */

    init_ui(&ui,&b);
    init_tune(&head,&cur,&new);

    init_total(display,&ui,&b);
	init_page(display,ui,&pg);
    init_offsets(display,ui,&m,b);
    init_margins(ui,&m);

    init_audio(&audio);

	init_font(&font);

    blit_lines(background,ui,m);
    blit_buttons(background,ui,m,b);
    blit_icons(background,ui,m,b);

	restore = SDL_ConvertSurface(background,background->format,DEPTH); /* create a copy of the background */
    SDL_BlitSurface(background,NULL,display,NULL);
    SDL_UpdateRect(display,0,0,0,0);
  
    /* TEST. A.K.A. startup sound */
    sounds[0].data = NULL;
	memset(sounds,0,sizeof(sound_t));
	memset(voice,0,sizeof(voice_t));
    load_audio(&audio,sounds);

	/*
    process_audio(&voice[0],&sounds[0],1,1.0);
    if(voice[0].data == NULL) {
        printf("ouch.\n");
    }
	*/

	do_print("1234567890",100,100,font,display);


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
                            pitch -= 0.5;
                            process_audio(&voice[0],&sounds[0],vol,pitch);
							printf("pitch; %.2f\n",pitch);
                            SDL_Delay(200);
                            break;
                        case SDLK_UP:
                            pitch += 0.5;
                            process_audio(&voice[0],&sounds[0],vol,pitch);
							printf("pitch; %.2f\n",pitch);
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
                            b.active = BUTTON_NONE;
                            printf("b.active: %d\n",b.active);
                            /* trigger a blit to get it actually disappear on the screen */
                            update_display(background,display,pos);
                            /* sort all nodes so they can be played in order */
                            head = msort_tune(head,n);
                            /* play them! */
                            play_tune(cur,head,n,sounds,TEMPO);
                            printf("n: %d\n",n);
                            break;
                        case SDLK_s:
                            head = msort_tune(head,n);
                            cur = head;
                            print_tune(&head,&cur);
                            break;
                        case SDLK_l:
                            b.active = BUTTON_NONE;
							/* toggle looping */
                            if(loop_tune == 0) {
                                loop_tune = 1;
                            }
                            else { loop_tune = 0; }

                            update_display(background,display,pos);
                            head = msort_tune(head,n);
                            printf("loop: %d\n",loop_tune);
                            break;
                        case SDLK_F5:
                            head = msort_tune(head,n);
                            save_tune(&head,&cur);
                            break;
						case SDLK_F12:
							break;
						case SDLK_z:
							/* previous page */
							if(pg.num <= 0) {
								pg.num = pg.num;
							}
							else { 
								pg.before = pg.num;
								pg.num--;
							}
							break;
						case SDLK_x:
							/* next page */
							pg.before = pg.num;
							pg.num++;
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
                            button_click(event,display,&ui,m,&b);
                            /* update the clipping rectangle for the icon. but only if the active button isn't none */
                            update_clip(ui,b.active,&clip);
                            /* check if the current positon is in the snap grid zone */
                            if(check_bounds(event,display,ui,m,b) == OK) {
                                update_pos(event,ui,m,&pos);
                                update_rel(event,ui,m,&rel);
                                /* initalize the members of the current node with the relative values */
                                store_tune(&cur, rel.x, rel.y, b.active, pg.num);
                                /* create the next node */
                                create_tune(&cur,&new);
                                n++;
                                /* make the button click actually appear on screen */
                                blit_click(background,ui,pos,clip);
                            }
                            printf("#: %d\n",b.active);
                            printf("nodes: %d\n",n);
                            break;
                        case SDL_BUTTON_RIGHT:
                            /* set button to none on right click */
                            b.active = BUTTON_NONE;
							update_pos(event,ui,m,&pos);
							update_rel(event,ui,m,&rel);
                            delete_tune(&head, &cur, &n, rel.x, rel.y);
                            update_display(restore,background,pos);
                            update_display(background,display,pos);
                            printf("#: %d\n",b.active);
                            break;
                        default:
                            break;
                    }
                /* mouse motion handling */
                case SDL_MOUSEMOTION:
                    if(detect_motion(event,ui,b,pos) == 1) {
                        update_display(background,display,pos);
                        printf("%d\n",blit++);
                        update_pos(event,ui,m,&pos);
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

		/* printf("x: %d\ty: %d\n",event.motion.x, event.motion.y); */

		if(loop_tune == 1) {
			play_tune(cur,head,n,sounds,TEMPO);
		}

		if(pg.num != pg.before) {
			pg.idx = pg.num * pg.width;
			clear_page(restore,background,display);

			printf("pg.idx %d\n",pg.idx);

			cur = head;
			while(cur->next != NULL) {
				if(cur->x > pg.idx && cur->x < pg.idx + pg.width) {
					update_page_pos(cur->x,cur->y,ui,m,pg,&pos);
					update_clip(ui,cur->i,&clip);

					SDL_BlitSurface(ui.icon,&clip,background,&pos);
				}
				cur = cur->next;
			}
			SDL_BlitSurface(background,NULL,display,NULL);
			SDL_Flip(display);
			pg.before = pg.num;
		}
       
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
    SDL_FreeSurface(ui.icon);
    SDL_FreeSurface(ui.play);
    /* SDL_FreeSurface(ui.icon_pressed); */
    SDL_FreeSurface(font);

    /* offload main surfaces */
    SDL_FreeSurface(restore);
    SDL_FreeSurface(background); 
    SDL_FreeSurface(display);
    SDL_FreeWAV(sounds[0].data);

    SDL_Quit();
    printf("done.\n");

    return(0);

}
