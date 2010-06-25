#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h> /* audio handling */

#include "define.h"
#include "config.h"
#include "sdl.c"
#include "pompcore.c"
#include "tune.c"


int main (int argc, char *argv[])
{
    SDL_Surface *surface;
    SDL_Surface *background;

    SDL_Event event;

    Uint32 frame; /* holds frame time */
    int loop = 1;
    int blit = 0;

    int i;

    char *sounds[] = {"default/04.wav","default/03.wav"};
    Mix_Chunk wave[2];

    face ui;

    node *first;
    node *cur;
    node *new;
    node *last;

    node *del;

    SDL_Rect pos;
    SDL_Rect old;
    SDL_Rect clip;

    atexit(SDL_Quit);

    /* init audio */
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,512);
    for(i=0;i<2; i++) {
        Mix_LoadWAV(sounds[i]);
    }

    /* let's go */
    surface = init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT);
    background = init_background(SCREEN_WIDTH,SCREEN_HEIGHT,BACKGROUND);
    /* load images */
    ui.button = load_image(BUTTON);
    ui.button_pressed = load_image(BUTTON_PRESSED);
    ui.icon = load_image(ICON);
    ui.play = load_image(PLAY);
    ui.line = load_image(LINE);

    ui.active = BUTTON_NONE;

    init_num(surface,&ui);
    init_offsets(surface,&ui);


    blit_lines(background,ui);
    blit_ui(background,ui);
    SDL_BlitSurface(background,NULL,surface,NULL);
    SDL_UpdateRect(surface,0,0,0,0);

    first = (node *)malloc(sizeof(node));
    cur = first;    
    cur->next = NULL;
    /* AUDIO ? */
    
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
                            break;
					}
                /* mouse button handling */
                case SDL_MOUSEBUTTONDOWN:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            ui.active = button_click(surface,event,ui);
                            update_clip(ui,&clip);
                            /*
                            if( event.button.y > ui.yoff 
                                && ui.active >  -1) {
                                SDL_BlitSurface(ui.icon,&clip,surface,&pos);
                                cur->x = event.button.x;
                                cur->y = event.button.y;
                                cur->i = ui.active;
                                new = (node *)malloc(sizeof(node));
                                new->next = NULL;
                                cur->next = new;
                                cur = new;
                            }
                            */
                            printf("bn: %d\n",ui.active);
                            break;
                        case SDL_BUTTON_RIGHT:
                            /* set button to none on right click */
                            ui.active = BUTTON_NONE;
                            SDL_BlitSurface(background,&pos,surface,&pos);
                            SDL_UpdateRect(surface,pos.x,pos.y,pos.w,pos.h);
                            printf("bn: %d\n",ui.active);
                            break;
                    }
                /* mouse motion handling */
                case SDL_MOUSEMOTION:
                    /* ugly */
                    if( (event.motion.x / ui.icon->h) * ui.icon->h != pos.x
                        || (event.motion.y / ui.icon->h) * ui.icon->h != pos.y ) {
                        SDL_BlitSurface(background,&pos,surface,&pos);
                        SDL_UpdateRect(surface,pos.x,pos.y,pos.w,pos.h);
                        update_pos(event,ui,&pos);
                        draw_cursor(surface,pos,ui);
                        printf("blit %d\n",blit++);
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
        SDL_UpdateRect(surface,0,0,0,0);
        */

		/* limit framerate */
		frame = SDL_GetTicks() - frame;
		if(frame < 1000/FRAMERATE) {
			SDL_Delay((1000/FRAMERATE) - frame);
		}


	/*main loop ends here. */
	}

    cur = first;
    while(cur->next != NULL) {
        printf("x: %d\ty: %d\ti: %d\n",cur->x,cur->y,cur->i);
        cur = cur->next;
    }

    printf("cleaning up... ");
    cur = first;
    while(cur->next != NULL) {
        del = cur->next;
        free(cur);
        cur = del;
    }
    /* offload ui */
    SDL_FreeSurface(ui.line);
    SDL_FreeSurface(ui.button);
    SDL_FreeSurface(ui.button_pressed);
    SDL_FreeSurface(ui.icon);
    /* SDL_FreeSurface(ui.icon_pressed); */

    /* offload main surfaces */
    SDL_FreeSurface(background); 
    SDL_FreeSurface(surface);

    for(i=0;i<2;i++) {
        Mix_FreeChunk(

    printf("done.\n");
    return(0);

}
