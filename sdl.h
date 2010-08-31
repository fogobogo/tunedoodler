SDL_Surface* init_sdl(unsigned int screen_width, unsigned int screen_height);
SDL_Surface* init_background(unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b);
SDL_Surface* load_image(const char *filename);
void set_colour(SDL_Surface *dst, int r, int g, int b);
