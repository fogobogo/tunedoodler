
SDL_Surface* init_background(unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b);
void init_ui(theme_t *ui, button_t *b);
void init_total(SDL_Surface *display, theme_t *ui, button_t *b);
void init_offsets(SDL_Surface *display, theme_t ui, metric_t *m, button_t b);
void init_margins(theme_t ui, metric_t *m);
