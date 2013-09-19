/*
 * Copyright (C) 2003-2013 Misha Nasledov <misha@nasledov.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
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
