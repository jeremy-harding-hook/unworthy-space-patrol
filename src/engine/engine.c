/* needed for nanosleep() et al */
#define _POSIX_C_SOURCE 199309L

#include <limits.h>
#include <time.h>
#include <curses.h>
#include "engine.h"
#include "memory.h"
#if COLLISION_DETECTION
#include "collisions.h"
#endif
/* TODO: implement main game loop, moving all the objects and checking for
 * collsions. Skip the collision check if not necessary. Also expose a method to
 * check for collisions with a specific game object. */

struct upoint screen_dimensions;
struct game_object *initial_game_object = NULL;

#define translate_position(position, translation) \
	do{ \
	position.x = (short unsigned int)(((long)position.x + (long)translation.x) \
		% screen_dimensions.x); \
	position.y = (short unsigned int)(((long)position.y + (long)translation.y) \
		% screen_dimensions.y); \
	} while(0)

int (*game_loop_continue)(void);

#define mvaddgobj(object) \
	mvaddch(screen_to_board(object->location.y), \
			screen_to_board(object->location.x), \
			object->character)
	
#if COLLISION_DETECTION
#define render_game_object(object) \
	do{ \
		if(!detect_collision(object)) \
		mvaddgobj(object); \
	} while(0)
#else
#define render_game_object(object) mvaddgobj(object)
#endif
			
void run_game(void){
	struct game_object *current_object;
	struct timespec clock_delay;

	clock_delay.tv_sec = 0;
	clock_delay.tv_nsec = MAIN_GAME_CLOCK_NSEC;
	while((*game_loop_continue)()){ 
		clear();
		/* clear_objects(initial_game_object); */
		update_screen_dimensions();

		current_object = initial_game_object;
		while(current_object != NULL){
			translate_position(
					current_object->location, current_object->velocity);
/* TODO: somehow compile an array of object pointers for each collision point? 
 * then that can be the param of the function to apply for each collision...
 * array of array pointers, so that I can just extend each one as needed?
 * Probably a terrible way of doing things, due to ever-growing arrays :/
 * Double layer of linked lists? That could work... outer one could even contain
 * board coordinates, to 8-bit precision... anyway, render and determine
 * collisions by checking cells first. Then the function should output a pointer
 * to the object to be rendered on top, so render all the return values with no
 * checking after. If return value is null, write a blank to the space. */
			render_game_object(current_object);
			current_object = current_object->next;
		}
#if COLLISION_DETECTION
		resolve_collisions();
#endif
		refresh();
		nanosleep(&clock_delay, NULL);
	}
}
