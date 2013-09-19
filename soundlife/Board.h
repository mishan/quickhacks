/*
 * Copyright (C) 2005-2013 Misha Nasledov <misha@nasledov.com>
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
#ifndef BOARD_H
#define BOARD_H

class Board {
public:
	Board (int width, int height);
//	Board (const Board &board);
	~Board (void);

	void clear (void);
	void randomize (void);
	void iterate (void);
	void print (void);
	bool **board (void) const { return board_; }
private:
	int neighbors (int x0, int y0);
	bool **createBoard (void);
	void copyBoard (bool **dst, bool **src);

	bool **board_;
	bool **tmpBoard_;
	int width_, height_;
	int iter_;
};

#endif /* BOARD_H */
