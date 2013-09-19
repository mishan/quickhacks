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
#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include "cannon.h"
#include "ui.h"

/* multiply degrees by this to convert them to radians */
#define DEGTORAD (double)(M_PI)/180

/* multiply radians by this to convert them to degrees */
#define RADTODEG (double)180/(M_PI)

/* our angle and force */
int angle = 60;
int force = 120;

struct vector {
	double net, x, y;
	double angle;
};

static struct vector *make_vector(double net, double angle)
{
	struct vector *f = malloc(sizeof(struct vector));

	f->net = net;
	f->angle = angle;
	f->x = net*cos(angle*DEGTORAD);
	f->y = net*sin(angle*DEGTORAD);

	return f;
}

static double distance_fallen (double v0, double t, double a)
{
	double ret = ((v0*t)+((a/2)*(t*t)));

	return ret>=0?ret:0;
}

static double out_of_bounds(double dx)
{
	double t;

	for(t = 0; t < WIDTH; t+=.1)
	{
		if(t*dx > WIDTH)
		{
			return t-.1;
		}
	}
	return t;
}

void draw_trajectory(double a, double n)
{
	struct vector *f;
	double m, t;

	f = make_vector(n, a);
	m = out_of_bounds(f->x);

	for(t = 0; t<=m; t+=.1)
	{
		setpixel(t*f->x, HEIGHT-distance_fallen(f->y, t, -9.81), 255);
		redraw();
	}
}
