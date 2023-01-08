#ifndef ENGINE_INCLUDED
#define ENGINE_INCLUDED

#ifdef ENGINE_SETTINGS_FILE
#include ENGINE_SETTINGS_FILE
#endif

/* Set the default settings */
#ifndef COLLISION_DETECTION
#define COLLISION_DETECTION TRUE
#endif

#include <curses.h>
#include "memory.h"

#ifndef MAIN_GAME_CLOCK_NSEC
/* 50 milliseconds */
#define MAIN_GAME_CLOCK_NSEC 50000000
#endif

/* Directions */
#define UP 		0
#define DOWN 	1
#define LEFT 	2
#define RIGHT 	3

#define screen_to_board(x) ((unsigned char)((x) >> 8))
/* This works because the shiftleft has an integer operation */
#define board_to_screen(x) ((short unsigned int)((x) << 8))

#define BOARD_BITMASK (((1U << 8) - 1) << 8)
#define are_board_location_equivalent(a,b) \
	(!(((a).x ^ (b).x) & BOARD_BITMASK || (((a).y ^ (b).y) & BOARD_BITMASK)))
#define have_same_board_location(a,b) \
	(are_board_location_equivalent((a)->location, (b)->location))

/**
 * The screen dimensions in subchars, i.e. with full precision.  In the case of
 * a screen dimension large enough to be considered infinite, the corrisponding
 * value shall be set to 0. An example could be when the actual value would be
 * greater than UINT_MAX.
 */
extern struct upoint screen_dimensions;

#define update_screen_dimensions() do{ \
	screen_dimensions.x = screen_to_board(UINT_MAX) >= (unsigned)COLS \
		? board_to_screen((unsigned)COLS) : 0; \
	screen_dimensions.y = screen_to_board(UINT_MAX) >= (unsigned)LINES \
		? board_to_screen((unsigned)LINES) : 0; \
	} while(0)

extern int (*game_loop_continue)(void);

/**
 * A pointer to the first game object to be updated. To add other game objects,
 * just insert them in the linked list via game_object->next
 */
extern struct game_object *initial_game_object;

void run_game(void);
#endif
