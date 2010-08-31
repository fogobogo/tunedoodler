
void update_display(SDL_Surface *surface, SDL_Surface *display, SDL_Rect pos);
void update_clip(theme_t ui, button_t b, SDL_Rect *clip);
void update_pos(SDL_Event event, theme_t ui, metric_t m, SDL_Rect *pos);
void update_rel(SDL_Event event, theme_t ui, metric_t m, SDL_Rect *rel);
