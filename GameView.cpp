#include "GameView.h"
#include "Game.h"
#include <cstring>
#include <iostream>

GameView::GameView(short width, short height)
	: screen_width(width), 
	  screen_height(height),
      actual_width(width * PIXEL_WIDTH),
	  actual_height(height) {
	h_console_out = GetStdHandle(STD_OUTPUT_HANDLE);

	short cx = screen_width / 2, cy = screen_height / 2;

	field_bounds  = SMALL_RECT{ 
		(short)(cx - 6), 
		(short)(cy - 6),
		(short)(cx + 5),
		(short)(cy + 5)};

	window = SMALL_RECT{0, 0, (short)(actual_width - 1), (short)(actual_height - 1)};

	SetConsoleWindowInfo(h_console_out, true, &window);

	buffer = new CHAR_INFO[actual_width * actual_height];
	memset(buffer, 0, sizeof(CHAR_INFO) * actual_width * actual_height);

	drawGUI();
	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
}

void GameView::clearText() {
	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
	WriteConsoleOutput(h_console_out, buffer, { actual_width, actual_height }, { 0, 0 }, &window);
	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
};

void GameView::update(Field field) {
	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
	for (short x = 0; x < field_bounds.Right - field_bounds.Left - 1; ++x) {
		for (short y = 0; y < field_bounds.Bottom - field_bounds.Top - 1; ++y) {
			CellWrapper cw = getCellView(field[y * Game::WIDTH + x]);
			drawPixel(field_bounds.Left + 1 + x, field_bounds.Top + 1 + y, cw.color, cw.c1, cw.c2);
		}
	}

	WriteConsoleOutput(h_console_out, buffer, { actual_width, actual_height }, { 0, 0 }, &window);
	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
}

GameView::CellWrapper GameView::getCellView(Cell cell) {
	short color = COLOR::B_BLACK | COLOR::F_WHITE, c1 = '?', c2 = '?';
	
	std::pair<Cell::State, Team> info = cell.getInfo();

	switch (info.first) {
	case Cell::State::EMPTY: {
		c1 = c2 = 0x0000;
		return CellWrapper{ color, c1, c2 };
	}
	case Cell::State::DEAD: {
		c1 = '>';
		c2 = '<';
		break;
	}
	case Cell::State::ALIVE: {
		c1 = c2 = 0x0000;
		break;
	}
	default:
		break;
	}

	switch (info.second) {
	case Team::RED: {
		color = COLOR::B_RED | COLOR::F_BLACK;
		break;
	}
	case Team::BLUE: {
		color = COLOR::B_BLUE | COLOR::F_BLACK;
		break;
	}
	case Team::NONE: {
		color = COLOR::B_BLACK | COLOR::F_WHITE;
		break;
	}
	default:
		break;
	}

	return CellWrapper{ color, c1, c2 };
}

void GameView::drawPixel(short x, short y, short color, short c1, short c2) {
	x *= PIXEL_WIDTH;
	if (x >= 0 && x < actual_width && y >= 0 && y < actual_height) {
		buffer[y * actual_width + x + 1].Char.UnicodeChar = c2;
		buffer[y * actual_width + x + 1].Attributes = color;
		buffer[y * actual_width + x].Char.UnicodeChar = c1;
		buffer[y * actual_width + x].Attributes = color;
	}
}

void GameView::drawGUI() {
	for (short x = field_bounds.Left; x <= field_bounds.Right; ++x) {
		short c = '0' + x - field_bounds.Left - 1;
		c = (x == field_bounds.Left || x == field_bounds.Right ? 0x0000 : c);
		drawPixel(x, field_bounds.Top, 112,	   0x0000, c);
		drawPixel(x, field_bounds.Bottom, 112, 0x0000, c);
	}
	for (short y = field_bounds.Top; y <= field_bounds.Bottom; ++y) {
		short c = 'a' + y - field_bounds.Top - 1;
		c = (y == field_bounds.Top || y == field_bounds.Bottom ? 0x0000 : c);
		drawPixel(field_bounds.Left, y, 112,  c, 0x0000);
		drawPixel(field_bounds.Right, y, 112, c, 0x0000);
	}

	WriteConsoleOutput(h_console_out, buffer, { actual_width, actual_height}, {0, 0}, &window);

	SetConsoleCursorPosition(h_console_out, COORD{ 0, 0 });
}
