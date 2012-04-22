/* Copyright (C) 2005 Misha Nasledov */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>

#include "Board.h"

Board::Board (int width, int height)
	: width_(width), height_(height), iter_(0)
{
	board_ = createBoard();
	tmpBoard_ = createBoard();

	clear ();
}

Board::~Board (void)
{
	/* free */
}

void Board::clear (void)
{
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			board_[y][x] = false;
		}
	}
}

void Board::randomize (void)
{
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			board_[y][x] = rand()%2 ? true : false;
		}
	}
}

void Board::iterate (void)
{
	copyBoard(tmpBoard_, board_);

	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			int neighborsNum = neighbors(x, y);

			if (board_[y][x])
			{
				if (neighborsNum > 3 || neighborsNum < 2)
					tmpBoard_[y][x] = false;
			}
			else
			{
				if (neighborsNum == 3)
					tmpBoard_[y][x] = true;
			}
		}
	}

	copyBoard(board_, tmpBoard_);
	iter_++;
}

void Board::print (void)
{
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
//			mvprintw(y, x, "%c", board_[y][x]*'*' | ' '); // slower??
			mvprintw(y, x, "%c", board_[y][x] ? '*' : ' ');
		}
	}
	mvprintw(height_-1, width_-7, "%06d", iter_);
	refresh();
}

int Board::neighbors (int x0, int y0)
{
	int cnt = 0;

	for (int y = y0 - 1; y <= y0 + 1; y++)
	{
		for (int x = x0 - 1; x <= x0 + 1; x++)
		{
			if (((x < 0) || (x >= width_)) ||
				((y < 0) || (y >= height_)) ||
				((x == x0) && (y == y0)))
				continue;

			if (board_[y][x])
				cnt++;
		}
	}

	return cnt;
}

bool **Board::createBoard (void)
{
	bool **board = new bool*[height_];

	for (int y = 0; y < height_; y++)
	{
		board[y] = new bool[width_];
	}

	return board;
}

void Board::copyBoard (bool **dest, bool **src)
{
	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			dest[y][x] = src[y][x];
		}
	}
}
