#!/bin/sh

CC="g++"
SOURCES="main.cpp midi.cpp Board.cpp"
BINARY="soundlife"
CFLAGS="-g -Wall"
LDFLAGS="-lncurses -lasound"

$CC $CFLAGS $SOURCES $LDFLAGS -o $BINARY