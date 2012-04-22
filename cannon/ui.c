#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "cannon.h"
#include "ui.h"

/* this is our screen */
SDL_Surface *surf = NULL;

/* this is our off-screen buffer */
SDL_Surface *vsurf = NULL;

/* our window */
GtkWidget *window = NULL;
static GtkWidget *vbox = NULL;

/* Event handlers -- the configure_event handler is very important! */
static gint configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
	if(event->width <= WIDTH && event->height <= HEIGHT)
		surf = SDL_SetVideoMode(event->width, event->height, 0, 0);
	redraw();
	return TRUE ;
}

static gint delete_event (GtkWidget *widget, GdkEventConfigure *event)
{
	gtk_main_quit();
	return TRUE ;
}


static void QuitGame(gpointer callback_data, guint callback_action, 
					 GtkWidget *widget)
{
	gtk_main_quit();
}

static GtkItemFactoryEntry menu_items[] = {
   {"/_File",			NULL,		0,		0, "<Branch>" },
   {"/_File/_Quit",		"<control>Q",	QuitGame,	0 },
};

static void create_menus(GtkWidget *window)
{
	GtkAccelGroup *accel_group;
	GtkItemFactory *item_factory;

	accel_group = gtk_accel_group_new();
	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<menu>", accel_group);
	gtk_item_factory_create_items(item_factory, 2, menu_items, NULL);
	gtk_accel_group_attach(accel_group, GTK_OBJECT(window));
	gtk_window_set_title(GTK_WINDOW(window), "Cannon");
	gtk_container_border_width(GTK_CONTAINER(window), 0);
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_box_pack_start(GTK_BOX(vbox), gtk_item_factory_get_widget(item_factory, "<menu>"), FALSE, FALSE, 0);
}

static void set_angle(GtkAdjustment *angdj, gpointer d)
{
	angle = angdj->value;
}

static void set_force(GtkAdjustment *fordj, gpointer d)
{
	force = fordj->value;
}

static void draw_stuff(GtkWidget *calculatebtn, gpointer d)
{
	draw_trajectory(angle, force);
}

static void create_controls(GtkWidget *window)
{
	GtkWidget *anglebtn;
	GtkWidget *forcebtn;
	GtkAdjustment *angdj;
	GtkAdjustment *fordj;
	GtkWidget *anvbox;
	GtkWidget *fovbox;
	GtkWidget *anlabel;
	GtkWidget *folabel;
	GtkWidget *hbox;
	GtkWidget *calculatebtn;
	
	angdj = (GtkAdjustment *)gtk_adjustment_new(angle, 1, 180, 1, 10, 10);
	gtk_signal_connect(GTK_OBJECT(angdj), "value-changed", set_angle, 0);

	fordj = (GtkAdjustment *)gtk_adjustment_new(force, 1, 5000, 5, 10, 10);
	gtk_signal_connect(GTK_OBJECT(fordj), "value-changed", set_force, 0);

	anglebtn = gtk_spin_button_new(angdj, 1, 0);
	anvbox = gtk_vbox_new(0, 0);
	anlabel = gtk_label_new("Angle");
	gtk_box_pack_start(GTK_BOX(anvbox), anglebtn, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(anvbox), anlabel, 0, 0, 0);

	forcebtn = gtk_spin_button_new(fordj, 5, 0);
	fovbox = gtk_vbox_new(0, 0);
	folabel = gtk_label_new("Force");
	gtk_box_pack_start(GTK_BOX(fovbox), forcebtn, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(fovbox), folabel, 0, 0, 0);

	hbox = gtk_hbox_new(0, 0);
	gtk_box_pack_start(GTK_BOX(hbox), anvbox, 0, 0, 50);
	gtk_box_pack_start(GTK_BOX(hbox), fovbox, 0, 0, 0);

	calculatebtn = gtk_button_new_with_label("Calculate");
	gtk_signal_connect(GTK_OBJECT(calculatebtn), "clicked", 
					   GTK_SIGNAL_FUNC(draw_stuff), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), calculatebtn, 0, 0, 50);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);

	gtk_widget_set_uposition(hbox, 20, HEIGHT+20);

	gtk_widget_show_all(window);
}

void redraw (void)
{
	SDL_BlitSurface(vsurf, NULL, surf, NULL);
	SDL_Flip(surf);
}

void redraw_portion(int x, int y, int w, int h)
{
	SDL_Rect r;

	r.x = (x>=0)?x:0;
	r.y = (y>=0)?y:0;
	r.w = w;
	r.h = h;

	SDL_BlitSurface(vsurf, &r, surf, &r);
	SDL_UpdateRect(surf, r.x, r.y, w, h);
}

void setpixel(int x, int y, int c)
{
	char *p;
	
	if((x >= WIDTH) || (y >= HEIGHT) || (x < 0) || (y < 0))
		return;

	p = vsurf->pixels+y*vsurf->pitch+x*vsurf->format->BytesPerPixel;
	SDL_LockSurface(vsurf);
	*p = c;
	SDL_UnlockSurface(vsurf);
}

int getpixel (int x, int y)
{
	char *p, val;

	SDL_LockSurface(vsurf);
	p = vsurf->pixels+y*vsurf->pitch+x*vsurf->format->BytesPerPixel;
	val = *p;
	SDL_UnlockSurface(vsurf);

	return val;
}

int running (void)
{
	SDL_Event e;

	if(SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return 27;
			break;
		case SDL_KEYUP:
			return e.key.keysym.sym;
			break;
		default:
			return 0;
			break;
		}
	}
	
	return 0;
}

/* yay, init the SDL stuff =) */
int init_ui (void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, WIDTH, HEIGHT+100);
	gtk_widget_realize(window);

	create_menus(window);
	create_controls(window);

	/* Add event handlers -- the configure_event handler is important! */
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
	gtk_signal_connect(GTK_OBJECT(window), "configure_event", GTK_SIGNAL_FUNC(configure_event), 0);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(delete_event), 0);
	
	/* Hack to get SDL to use GTK window */
	{ char SDL_windowhack[32];
		sprintf(SDL_windowhack,"SDL_WINDOWID=%ld",
			GDK_WINDOW_XWINDOW(window->window));
		putenv(SDL_windowhack);
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	if(!SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, 0)) {
		fprintf(stderr, "Could not set %dx%dx%d video mode: %s\n", 
				WIDTH, HEIGHT, DEPTH, SDL_GetError());
	}

	/* ignore mouse events */
	SDL_EventState(SDL_MOUSEMOTION,     SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP,   SDL_IGNORE);

	/* set window title, icon is NULL */
	SDL_WM_SetCaption("Cannon", NULL);

	/* set our SDL surface */
	surf = SDL_GetVideoSurface();

	/* create a new surface with the same format as our screen */
	vsurf = SDL_DisplayFormat(surf);
	
	/* seed the random number generator */
	srand(time(NULL));
	
	/* everything's okay! */
	return 0;
}
