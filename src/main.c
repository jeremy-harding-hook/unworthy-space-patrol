#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <locale.h>
#include "stringing.h"

int main(void){
	unsigned char centre_x;
	unsigned char centre_y;
	printf("Starting...\n");
	
	/* initialise ncurses */
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	clear();

	centre_x = (unsigned char)(COLS-1)/2;
	centre_y = (unsigned char)(LINES-1)/2;
	mvaddch(centre_y, centre_x, '#');
	refresh();
	sleep(3);
	endwin();
	printf("Exiting\n");
	return 0;
}
