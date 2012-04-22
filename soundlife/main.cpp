/* Copyright (C) 2005 Misha Nasledov */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#include <string>
#include "Board.h"
#include "midi.h"

#define EXIT 'q'
//#define MIDISIZE 64

#define SCALE_NOTES 50
#define START_NOTE 12

bool midiNotes[SCALE_NOTES];
//bool midiBuf[SCALE_NOTES];
AlsaMidi *MidiMaster = NULL;


char scaleCycleAdder[] = {2, 1, 2, 2, 1, 2, 2};
char scaleCycleAdderLen = sizeof(scaleCycleAdder)/sizeof(char);
char scaleNotes[SCALE_NOTES];

void generateScaleNotes (void)
{
	int cycle_index = 0;

	int n = START_NOTE;

	for (int i = 0; i < SCALE_NOTES; i++)
	{
		scaleNotes[i] = n;
		n += scaleCycleAdder[cycle_index];
		cycle_index = (cycle_index + 1) % scaleCycleAdderLen;
	} 
}

int process_keys (void)
{
	/* XXX */
	return getch();
}


void play (bool *midiBuf)
{
//	bool *midiBuf = getMidiBuffer(board, w, h);
	
	for (int i = 0; i < SCALE_NOTES; i++)
	{
#if 0
		if (midiBuf[i] == true && midiNotes[i] == false)
		{
//			printf("setting note %d on\n", i);
			MidiMaster->sendNote(scaleNotes[i], true);
		}
		else if (midiBuf[i] == false && midiNotes[i] == true)
		{
//			printf("setting note %d off\n", i);
			MidiMaster->sendNote(scaleNotes[i], false);
		}
#endif
//		if (midiBuf[i])
			MidiMaster->sendNote(scaleNotes[i], midiBuf[i]);
	}
	MidiMaster->flush();

	memcpy(midiNotes, midiBuf, SCALE_NOTES*sizeof(bool));

}



bool *getMidiBuffer (bool **buf, int w, int h, int &w0, int &h0)
{
	bool *midiBuf = new bool[SCALE_NOTES];
	int midiInd = 0;
	int x, y;

	for (y = h0; y < h; y++)
	{
		for (x = w0; x < w; x++)
		{
			midiBuf[midiInd++] = buf[y][x];

			if (midiInd == SCALE_NOTES)
				goto midiRet;
		}
	}

  midiRet:

	h0 = y;
	w0 = x;

	return midiBuf;
}

void sonify (bool **buf, int w, int h)
{
	int w0 = 0, h0 = 0;

	while ((w0 != w) && (h0 != h))
	{

		bool *midiBuf = getMidiBuffer(buf, w, h, w0, h0);
		play (midiBuf);
		usleep(12500);
	}
	
}

//void sonify (bool **board, int w, int h)

int main (int argc, char *argv[])
{
//	int w = 8, h = 8;
//	int w = 80, h = 72;

	initscr();
	cbreak();
	noecho();
//	nonl();
	nodelay(stdscr, TRUE);
//	intrflush(stdscr, FALSE);
//	keypad(stdscr, TRUE);

	MidiMaster = new AlsaMidi("soundlife", argv[1]);

	int w = COLS, h = LINES;
//	int w = 8, h = 8;

	Board board(w, h);

	srand(time(NULL));
	board.randomize();

	generateScaleNotes();

	while (process_keys() != EXIT)
//	while (1)
	{
		board.print();
		sonify(board.board(), w, h);
//		printf("\n\n");
		usleep(100000);
		board.iterate();
	}

	endwin();
}
