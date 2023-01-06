/* needed for nanosleep() et al */
#define _POSIX_C_SOURCE 199309L

#include <limits.h>
#include <time.h>
#include "engine.h"
#include "collisions.h"
/* TODO: implement main game loop, moving all the objects and checking for
 * collsions. Skip the collision check if not necessary. Also expose a method to
 * check for collisions with a specific game object. */

struct upoint screen_dimensions;
struct game_object *initial_game_object;

/* TODO: Determine if this is any faster than a normal clear() */
#if FALSE
static void clear_objects(struct game_object *object){
	while(object != NULL){
		mvaddch(screen_to_board(object->location.y),
				screen_to_board(object->location.x), ' ');
		object = object->next;
	}
}

#define get_sliver(width) (((UINT_MAX % (width))+1)==(width)?0: \
		((UINT_MAX % (width))+1))
/* TODO: Consider replacing all this with an int32 calculation? It would be
 * undoubtedly faster and probably wouldn't use more memory, considering that
 * sliver is already 16 bits at least */
static unsigned int translate_coordinate(unsigned int location, int velocity,
		unsigned int screen_dimension){
	register unsigned int result;
	if(!screen_dimension)
		return location + (unsigned)velocity;
	result = (location + (unsigned)velocity) % screen_dimension;
	if(velocity < 0)
		result -= get_sliver(screen_dimension);
	if(result < location)
		return result + get_sliver(screen_dimension);
	return result;
}
#undef get_sliver

struct upoint translate_position(struct upoint position,
		struct point translation){
#if TRUE
	position.x = (short unsigned int)((long)position.x + (long)translation.x)
		% screen_dimensions.x;
	position.y = (short unsigned int)((long)position.y + (long)translation.y)
		% screen_dimensions.y;
#else
	position.x = translate_coordinate(position.x, translation.x,
			screen_dimensions.x);
	position.y = translate_coordinate(position.y, translation.y,
			screen_dimensions.y);
#endif
	return position;
}
#endif

#define translate_position(position, translation) \
	do{ \
	position.x = (short unsigned int)(((long)position.x + (long)translation.x) \
		% screen_dimensions.x); \
	position.y = (short unsigned int)(((long)position.y + (long)translation.y) \
		% screen_dimensions.y); \
	} while(0)

int (*game_loop_continue)(void);

/* TODO: Convert to macro? */
void render_game_object(struct game_object *object){
#if COLLISION_DETECTION
	/* TODO: add the collision detection logic here */
#else
mvaddch(screen_to_board(object->location.y),
		screen_to_board(object->location.x),
		object->character);
#endif
}

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
		refresh();
		nanosleep(&clock_delay, NULL);
	}
}
