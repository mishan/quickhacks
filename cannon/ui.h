#ifndef UI_H
#define UI_H

/* this is our screen */
extern SDL_Surface *surf;

/* this is our off-screen buffer */
extern SDL_Surface *vsurf;

extern GtkWidget *window;

/* redraw the whole screen */
extern void redraw (void);

/* redraw a given rectangle */
extern void redraw_portion(int x, int y, int w, int h);

/* set a pixel (x, y) to c */
extern void setpixel(int x, int y, int c);

/* get the value of (x, y) */
extern int getpixel (int x, int y);

/* it'll return 27 if we should quit */
extern int running (void);

/* initialize the user interface */
extern int init_ui (void);

#endif
