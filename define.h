
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	480
#define DEPTH	32
#define SDL_FLAGS	SDL_SWSURFACE|SDL_RESIZABLE
#define SDL_INIT    SDL_INIT_VIDEO|SDL_INIT_AUDIO


#define TITLE "pompouser"

#define BUTTON_NONE     -1
#define MOTION          1
#define NO_MOTION       0
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


typedef struct tune_t {
    unsigned int x;
    unsigned int y;
    int i;
    struct tune_t *next;
} tune_t;


typedef struct {
    Uint8 *data;
    Uint32 length;
} sound_t;


typedef struct {
    Sint16 *data;
    int length;
    int pos;
    float f_pos; /* pos casted as float */
    int r_vol;
    int l_vol;
    float pitch;
} voice_t;

    voice_t voice[1];
