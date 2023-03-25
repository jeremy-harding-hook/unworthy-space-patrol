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

struct upoint screen_dimensions;
struct game_object *initial_game_object = NULL;

#define translate_position(position, translation) \
	do{ \
	position.x = (short unsigned int)((((long)position.x + (long)translation.x)\
		% screen_dimensions.x + screen_dimensions.x) % screen_dimensions.x); \
	position.y = (short unsigned int)((((long)position.y + (long)translation.y)\
		% screen_dimensions.y + screen_dimensions.y) % screen_dimensions.y); \
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
		update_screen_dimensions();
		mvaddch(screen_to_board(screen_dimensions.y - 1),
				screen_to_board(screen_dimensions.x - 1),
				'#');
		mvaddch(0,0,'1');
		mvaddch(0,screen_to_board(screen_dimensions.x - 1), '2');
		mvaddch(screen_to_board(screen_dimensions.y - 1), 0, '2');

		current_object = initial_game_object;
		while(current_object != NULL){
			translate_position(
					current_object->location, current_object->velocity);
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
