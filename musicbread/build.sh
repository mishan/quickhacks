#/bin/sh
# my lame build script because I was too lazy to write a Makefile

SRCS="main.cpp BreadNote.cpp BreadNoteCluster.cpp GenePool.cpp music-util.cpp"
CC=gcc
CFLAGS="-g -Wall"
BINARY=musicbread
LDFLAGS="-lstdc++ -ljdksmidi"

$CC $CFLAGS $SRCS -o $BINARY $LDFLAGS
