
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	480
#define DEPTH	32
#define SDL_FLAGS	SDL_SWSURFACE|SDL_RESIZABLE
#define SDL_INIT    SDL_INIT_VIDEO|SDL_INIT_AUDIO


#define TITLE "pompouser"

#define BUTTON_NONE     -1
#define MOTION          1
#define CLICK           1
#define OK              1
#define FAIL            0

typedef struct {
    unsigned int x;
    unsigned int y;
} point;

typedef struct {
    SDL_Surface *line;
    SDL_Surface *button;
    SDL_Surface *button_pressed;
    SDL_Surface *icon;
    /* SDL_Surface *icon_pressed */
    SDL_Surface *play;
    /* SDL_Surface *play_pressed; */
    int active; /* active button */
    int before;
    int total; /* total number of buttons */
} pompface;

typedef struct tune {
    unsigned int x;
    unsigned int y;
    int i;
    struct tune *next;
} tune;


