
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	480
#define DEPTH	32
#define SDL_FLAGS	SDL_SWSURFACE|SDL_RESIZABLE
#define SDL_INIT    SDL_INIT_VIDEO|SDL_INIT_AUDIO


#define TITLE "pompouser"

#define BUTTON_NONE     -1

typedef struct {
    unsigned int x;
    unsigned int y;
} point;

typedef struct {
    SDL_Surface *line;
    SDL_Surface *button;
    SDL_Surface *button_pressed;
    SDL_Surface *icon;
    /* SDL_Surface *icon_pressed = NULL; */
    SDL_Surface *play;
    /* SDL_Surface *play_pressed; */

    int xoff; /* horizontal offset */
    int yoff; /* vertical offset */
    int num; /* total number of buttons */
    int active; /* number of active button */
} face;

typedef struct node {
    int x;
    int y;
    int i;
    struct node *next;
} node;

