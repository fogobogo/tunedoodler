
/* create background with color */
SDL_Surface*
init_background(unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b)
{
    SDL_Surface *tmp;
    tmp = SDL_CreateRGBSurface(SDL_FLAGS,w,h,DEPTH,0,0,0,0);
    SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,r,b,g));
    return(tmp);
}
    
    
void
init_ui(pompface *ui)
{
    /* load images */
    ui->button = load_image(BUTTON);
    ui->button_pressed = load_image(BUTTON_PRESSED);
    ui->icon = load_image(ICON);
    ui->play = load_image(PLAY);
    ui->line = load_image(LINE);

    /* set active button to none */
    ui->active = BUTTON_NONE;
    ui->before = BUTTON_NONE;
}


void
init_total(SDL_Surface *display, pompface *ui)
{
    int buttons, icons; /* buttons, icons */

    buttons = ((display->w - ui->play->w) / ui->button->w);
    /* calculate the number of icons. this assumes the icon width equals the icon height */
    icons = ui->icon->w / ui->icon->h; 

    /* if there are less icons than buttons that fit on the screen blit only as many buttons as there are icons */
    if(buttons > icons) { ui->total = icons; }
    /* else blit as many buttons as possible which means there will be some icons hidden (or match the button number) */
    else { ui->total = buttons; }
}

void
init_offsets(SDL_Surface *display, pompface ui, point *offset)
{
    /* calculate the horizontal offset (left, right) relative to the window so the buttons are always centered on the screen */
    /* store the values so we can later figure out what button was pressed */
    offset->x = ((display->w - ui.play->w) - (ui.button->w * ui.total)) / 2; /* from left */
    offset->y = (display->h - (ui.icon->h * (LINE_NO * 2))) / 2 + (ui.icon->h / 2);
    printf("xoff: %d\n",offset->x);
    printf("yoff: %d\n",offset->y);
}

void
init_margins(pompface ui, point *margin)
{
    /* to center icons on buttons */
    margin->x = (ui.button->w - ui.icon->h) / 2;
    margin->y = (ui.button->h - ui.icon->h) / 2;
}
