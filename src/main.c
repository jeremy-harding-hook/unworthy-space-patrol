#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <locale.h>
#include "engine/engine.h"
#include "engine/memory.h"
#include "engine/collisions.h"

#define increment_within_bounds(x,y) (x == (y) ? x = 0 : x++)
#define decrement_within_bounds(x,y) (x == 0 ? x = (y) : x--)
#define LAST_COL ((unsigned char)(COLS-1))
#define LAST_LINE ((unsigned char)(LINES-1))

int handle_game_loop_top(void){
	register int input_char = getch();
	switch (input_char)
	{
#if FALSE
		case KEY_LEFT:
		case 'h':
			decrement_within_bounds(centre_x, LAST_COL);
			break;
		case KEY_RIGHT:
		case 'l':
			increment_within_bounds(centre_x, LAST_COL);
			break;
		case KEY_UP:
		case 'k':
			decrement_within_bounds(centre_y, LAST_LINE);
			break;
		case KEY_DOWN:
		case 'j':
			increment_within_bounds(centre_y, LAST_LINE);
			break;
#endif
		case 'q':
		case 'Q':
			return FALSE;
	}
	refresh();
	return TRUE;
}

chtype handle_collision(struct game_object_list_element *objects){
	return objects->object->character;
}

int main(void){
	printf("Starting...\n");

	/* initialise ncurses */
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	/* run the game */
	clear();
	refresh();
	game_loop_continue = &handle_game_loop_top;
	resolve_collision = &handle_collision;
#if FALSE
	initial_game_object = malloc(sizeof(struct game_object));
	initial_game_object->character = '#';
	initial_game_object->next = NULL;
	initial_game_object->velocity.x = 13;
	initial_game_object->velocity.y = 17;
#endif
	run_game();
	free(initial_game_object);

	/* cleanup and exit */
	endwin();
	printf("Exiting\n");
	return 0;
}
