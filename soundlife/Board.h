/* Copyright (C) 2005 Misha Nasledov */
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
