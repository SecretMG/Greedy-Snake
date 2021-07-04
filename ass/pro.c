#include "stdlib.h"
#include "time.h"
#include "windows.h"

extern int get_rand();
extern void moveCursor(int x, int y);
extern void sleep();

int get_rand() {
	int num;
	srand((unsigned)time(NULL));
	num = (rand() % (20 - 5) + 2) * 40 + (rand() % (40 - 5) + 2);
	return num;
}

void moveCursor(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
	return;
}

void sleep() {
	Sleep(50);
	return;
}

int turn() {
	if (_kbhit()) {
		char c = _getch();
		switch (c) {
		case 'w':
			return 1;
		case 's':
			return 2;
		case 'a':
			return 3;
		case 'd':
			return 4;
		default:
			return 0;
		}
	}
	return 0;
}