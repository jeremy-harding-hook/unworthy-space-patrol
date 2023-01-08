#ifndef MEMORY_H
#define MEMORY_H
#include <curses.h>

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
 * A structure suitable for creating linked lists of game objects.
 * This structure represents an element in such a list.
 */
struct game_object_list_element{
	struct game_object_list_element *next;
	struct game_object *object;
};

/**
 * Creates a new game object, with the values passed in as parameters. If in
 * doubt, next can be set to NULL. Though if that's unclear you may want to 
 * touch up on the documentation regarding game_objects in general.
 */
struct game_object *new_game_object(
		unsigned char character, struct game_object *next,
		unsigned short int location_x, unsigned short int location_y,
		short int velocity_x, short int velocity_y);

/**
 * Dispose of a game object. Note that this does not release the memory from the
 * process, but rather puts it in a state in which it can be reused. This has
 * the advantage of cutting down on uses of malloc and free, at the cost of
 * using unneeded memory when the demand is lower.
 */
void ditch_game_object(struct game_object *object);

/**
 * Gets a new game object list element, setting the values to the corrisponding
 * parameters. The struct is mallocked if there are none left in the pool to be
 * reused, but otherwise it's just recycled.
 */
struct game_object_list_element *new_game_object_list_element(
		struct game_object *object, struct game_object_list_element *next);

/**
 * Disposes of a game object list element. Note that the memory is not freed
 * from the process, but rather simply placed in a recycling pool to be reused.
 */
void ditch_game_object_list_element(struct game_object_list_element *element);

#endif
