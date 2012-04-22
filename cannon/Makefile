CC := gcc
PROGRAM := cannon 
SRCS := main.c cannon.c ui.c 
OBJS = $(SRCS:.c=.o)

INCLUDES := $(shell gtk-config --cflags) $(shell sdl-config --cflags)

CFLAGS := $(INCLUDES) -Wall \
	-g

LIBS := $(shell gtk-config --libs) $(shell sdl-config --libs)

MAGIC_DEPS := $(shell if [ ! -d .deps ]; then mkdir .deps > /dev/null 2>&1 || :; fi)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(DEFINES) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c
ifeq ($(CC), gcc)
		$(CC) $(CFLAGS) -Wp,-MMD,.deps/$<.dt -c $< -o $@
		@sed '/^[:\\ ]*$$/d;s/\([^\\]$$\)/\1 Makefile/' <.deps/$<.dt>.deps/$<.d && rm -f .deps/$<.dt
else
		@$(CC) $(CFLAGS) -M $< | sed 's@ /[^ ]*@@g;/^[:\\ ]*$$/d;s/\([^\\]$$\)/\1 Makefile' >.deps/$<.d
		$(CC) $(CFLAGS) -c $< -o $@
endif

-include .deps/*.d

clean: 
	rm -f $(OBJS) $(PROGRAM)
	rm -rf .deps

dist: clean
	-cd ..; tar c $(shell basename $(PWD)) | gzip -9 > $(PROGRAM).tar.gz

.PHONY: clean dist
