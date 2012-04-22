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
