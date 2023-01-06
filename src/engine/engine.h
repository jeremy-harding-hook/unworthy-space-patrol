#ifndef ENGINE_INCLUDED
#define ENGINE_INCLUDED

#include <stdlib.h>
#include <curses.h>
#ifdef ENGINE_SETTINGS_FILE
#include ENGINE_SETTINGS_FILE
#endif

/* Set the default settings */
#ifndef COLLISION_DETECTION
#define COLLISION_DETECTION TRUE
#endif

#ifndef MAIN_GAME_CLOCK_NSEC
/* 50 milliseconds */
#define MAIN_GAME_CLOCK_NSEC 50000000
#endif

/* Directions */
#define UP 		0
#define DOWN 	1
#define LEFT 	2
#define RIGHT 	3

/**
 * Represents a pair of unsigned x and y coordinates. These may be static (as
 * screen coordinates), or may represent a speed, for instance. When mapping to
 * screen, note that the first eight bits are taken as the screen coordinates,
 * allowing for eight bits of sub-character precision.
 */
struct upoint{
	/**
	 * Represents the horizontal component, typically pointing right.
	 */
	unsigned int x: 16;
	/**
	 * Represents the vertical component, typically pointing down.
	 */
	unsigned int y: 16;
};

/**
 * Represents a pair of signed x and y coordinates. These may be static (as
 * screen coordinates), or may represent a speed, for instance. When mapping to
 * screen, note that the first eight bits are taken as the screen coordinates,
 * allowing for eight bits of sub-character precision.
 */
struct point{
	/**
	 * Represents the horizontal component, typically pointing right.
	 */
	signed int x: 16;
	/**
	 * Represents the vertical component, typically pointing down.
	 */
	signed int y: 16;
};

#define screen_to_board(x) ((short unsigned int)((x) >> 8))
#define board_to_screen(x) ((short unsigned int)((x) << 8))

/**
 * Represents an object in-game. At present only one-character objects are
 * supported, so it's not possible to have a game object take up multiple spaces
 * on-screen. This may be changed at a later date.
 */
struct game_object{
	/**
	 * A pointer to the game object to be updated after this one.
	 */
	struct game_object *next;
	/**
	 * Represents the current location of the game object. Note the actual
	 * screen coordinates are obtained via passing these values through the
	 * screen_to_board(x) macro. In effect the highest eight bits represent the
	 * actual screen placement, and the rest are for extra precision.
	 */
	struct upoint location;
	/**
	 * Represents the current velocity of the game object. At every cycle of the
	 * main game loop, this is added to the location in order to move the
	 * object.
	 */
	struct point velocity;
	/**
	 * The character used to represent the game object. This contains both the
	 * character displayed as well as information regarding how it is styled, as
	 * supported by ncurses.
	 */
	chtype character;
};

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

/**
 * Translates the position passed in the first parameter by the amount specified
 * in the second and returns the result, with wraparound as appropriate.
 * \param[in] position The point to be modified.
 * \param[in] translation The amount by which to move the position.
 */
struct upoint translate_position(struct upoint position,
		struct point translation);

extern int (*game_loop_continue)(void);

/**
 * A pointer to the first game object to be updated. To add other game objects,
 * just insert them in the linked list via game_object->next
 */
extern struct game_object *initial_game_object;

void run_game(void);
#endif
