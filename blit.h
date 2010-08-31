
void blit_buttons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b);
void blit_icons(SDL_Surface *surface, theme_t ui, metric_t m, button_t b);
void blit_lines(SDL_Surface *surface, theme_t ui, metric_t m);
void blit_click(SDL_Surface *surface, theme_t ui, SDL_Rect pos, SDL_Rect clip);
void blit_cursor(SDL_Surface *display, SDL_Rect pos, SDL_Rect clip, theme_t ui);
