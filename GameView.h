#pragma once
#include "Game.h"
#include "windows.h"

enum COLOR {
	F_BLACK = 0,
	F_RED   = FOREGROUND_RED,
	F_BLUE  = FOREGROUND_BLUE,
	F_GREEN = FOREGROUND_GREEN,
	F_WHITE = F_RED | F_BLUE | F_GREEN,

	B_BLACK = 0,
	B_RED   = BACKGROUND_RED,
	B_BLUE  = BACKGROUND_BLUE,
	B_GREEN = BACKGROUND_GREEN,
	B_WHITE = B_RED | B_BLUE | B_GREEN
};

class GameView
{
	struct CellWrapper { short color; short c1; short c2; };
public:
	GameView() : GameView(60, 30) {};
	GameView(short width, short height);

	void clearText();
	void update(Field field);
private:
	void drawPixel(short x, short y, short color=COLOR::B_WHITE, short c1 = 0x2588, short c2 = 0x2588);
	void drawGUI();
	
	CellWrapper getCellView(Cell cell);

	const short PIXEL_WIDTH = 2;

	short actual_width;
	short actual_height;

	short screen_width;
	short screen_height;
	
	CHAR_INFO* buffer;
	SMALL_RECT window;

	SMALL_RECT field_bounds;

	HANDLE h_console_out;
};

