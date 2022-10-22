#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <locale.h>
#include "stringing.h"

#define increment_within_bounds(x,y) (x == (y) ? x = 0 : x++)
#define decrement_within_bounds(x,y) (x == 0 ? x = (y) : x--)
#define LAST_COL ((unsigned char)(COLS-1))
#define LAST_LINE ((unsigned char)(LINES-1))
int main(void){
	unsigned char centre_x;
	unsigned char centre_y;
	printf("Starting...\n");

	/* initialise ncurses */
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	clear();

	centre_x = LAST_COL/2;
	centre_y = LAST_LINE/2;
	mvaddch(centre_y, centre_x, '#');
	refresh();
	while(true){
		register int input_char = getch();
		mvaddch(centre_y, centre_x, ' ');
		switch (input_char)
		{
			case KEY_LEFT:
				decrement_within_bounds(centre_x, LAST_COL);
				break;
			case KEY_RIGHT:
				increment_within_bounds(centre_x, LAST_COL);
				break;
			case KEY_UP:
				decrement_within_bounds(centre_y, LAST_LINE);
				break;
			case KEY_DOWN:
				increment_within_bounds(centre_y, LAST_LINE);
				break;
			case 'q':
			case 'Q':
				goto QUIT;
		}
		mvaddch(centre_y, centre_x, '#');
		refresh();
	}
QUIT:
	endwin();
	printf("Exiting\n");
	return 0;
}
