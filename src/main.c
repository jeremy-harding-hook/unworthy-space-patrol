#include <limits.h>
#include <stdlib.h>
#include <curses.h>
#include <locale.h>
#include "engine/engine.h"
#include "engine/memory.h"
#include "engine/collisions.h"

#define increment_within_bounds(x,y) (x == (y) ? x = 0 : x++)
#define decrement_within_bounds(x,y) (x == 0 ? x = (y) : x--)
#define LAST_COL ((unsigned char)(COLS-1))
#define LAST_LINE ((unsigned char)(LINES-1))
#define rand_within_range(lower,upper) ((rand() % ((upper) - (lower) + 1)) + \
		(lower))

#define COLOR_PAIR_KABOOM 1
#define KABOOM_CHAR '@' | COLOR_PAIR(COLOR_PAIR_KABOOM);

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

void collide_objects(struct game_object *obj1, struct game_object *obj2,
		struct point *delta_v1, struct point *delta_v2){
	unsigned char m1 = obj1->character & A_CHARTEXT;
	unsigned char m2 = obj2->character & A_CHARTEXT;
		
	float coefficient_same = (float)(m1 - m2)/(m1 + m2);
	float coefficient_opposite = (float)m2*2/(m1 + m2);

	delta_v2->x = (short int)(obj1->velocity.x * coefficient_opposite +
			obj2->velocity.x * (coefficient_same - 1));
	delta_v2->y = (short int)(obj1->velocity.y * coefficient_opposite +
			obj2->velocity.y * (coefficient_same - 1));

	coefficient_same = -coefficient_same;
	coefficient_opposite = (float)m1*2/(m1 + m2);

	delta_v1->x = (short int)(obj2->velocity.x * coefficient_opposite +
			obj1->velocity.x * (coefficient_same - 1));
	delta_v1->y = (short int)(obj2->velocity.y * coefficient_opposite +
			obj1->velocity.y * (coefficient_same - 1));
}

chtype handle_collision(struct game_object_list_element *objects){
	unsigned int objects_count = 0;
	unsigned int i = 0;
	unsigned int j;
	struct game_object_list_element *pointer = objects;
	struct point *new_velocities;
	while(pointer)
	{
		objects_count++;
		pointer = pointer->next;
	}

	new_velocities = malloc(objects_count * sizeof(struct point));
	while(objects){
		pointer = objects->next;
		j=i+1;
		while(pointer){
			collide_objects(objects->object, pointer->object,
					&new_velocities[i], &new_velocities[j]);
			pointer = pointer->next;
			j++;
		}
		objects->object->velocity.x += new_velocities[i].x;
		objects->object->velocity.y += new_velocities[i].y;
		objects = objects->next;
		i++;
	}
	free(new_velocities);
	return KABOOM_CHAR;
}

struct game_object *get_random_game_object(struct game_object *next){
	struct game_object *object;
   	register int max_x =  screen_dimensions.x;
	register int max_y =  screen_dimensions.y;
	if(max_x > 0) max_x--;
	if(max_y > 0) max_y--;

	object = new_game_object(
			(unsigned char)rand_within_range(33, 126),
			next,
			(unsigned short)rand_within_range(0, max_x),
			(unsigned short)rand_within_range(0, max_y),
			(short)rand_within_range(-256, 256),
			(short)rand_within_range(-256, 256));
	return object;
}

void free_all_game_objects(struct game_object *object){
	struct game_object* next;
	while(object != NULL){
		next = object->next;
		free(object);
		object = next;
	}
}

int main(void){
	int game_objects_to_produce, nitems;
	printf("Starting...\n");
	printf("How many game objects shall I throw up there?\n");

	nitems = scanf("%d", &game_objects_to_produce);
	if (nitems == EOF || nitems == 0) {
		printf("Apparently none...bye bye.\n");
		return 1;
	} 
	if(game_objects_to_produce < 0){
		printf("Uhh...\n");
		return 1;
	}

	/* initialise ncurses */
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	/* colours */
	start_color();
	init_pair(COLOR_PAIR_KABOOM, COLOR_YELLOW, COLOR_BLACK);

	/* run the game */
	clear();
	refresh();

	update_screen_dimensions();
		mvaddch(screen_to_board(screen_dimensions.y - 1),
				screen_to_board(screen_dimensions.x - 1),
				'#');
		mvaddch(0,0,'1');
		mvaddch(0,screen_to_board(screen_dimensions.x - 1), '2');
		mvaddch(screen_to_board(screen_dimensions.y - 1), 0, '2');
		refresh();

	game_loop_continue = &handle_game_loop_top;
	resolve_collision = &handle_collision;

	initial_game_object = NULL;
	while(game_objects_to_produce-- > 0){
		initial_game_object = get_random_game_object(initial_game_object);
	}
	run_game();
	free_all_game_objects(initial_game_object);

	/* cleanup and exit */
	endwin();
	printf("Exiting\n");
	return 0;
}
