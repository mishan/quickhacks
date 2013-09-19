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
#include <stdio.h>
#include "ui.h"
#include "cannon.h"

static int idle_loop (gpointer data)
{
	if(running() == 27)
		gtk_main_quit();

	return 1;
}

int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	if((init_ui()) > 0)
		exit(1);

	gtk_idle_add(idle_loop, window);
	gtk_main();
	SDL_Quit();

	return 0;
}
