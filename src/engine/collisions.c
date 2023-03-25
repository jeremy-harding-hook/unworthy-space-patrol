#include "engine.h"
#include "memory.h"
#include "collisions.h"
#if COLLISION_DETECTION

static struct game_object_list_element *initial_colliding_game_object = NULL;

int detect_collision(struct game_object *object){
	if((mvinch(screen_to_board(object->location.y),
					screen_to_board(object->location.x)) & A_CHARTEXT) == ' ')
		return FALSE;
	{
		struct game_object_list_element *collided_object =
			initial_colliding_game_object;
		while(collided_object){
			if(have_same_board_location(collided_object->object, object)){
				while(collided_object->next && have_same_board_location(
							collided_object->next->object, object)){
					collided_object = collided_object->next;
				}
				collided_object->next = 
					new_game_object_list_element(object, collided_object->next);
				return TRUE;
			}
			collided_object = collided_object->next;
		}
	}
	{
		struct game_object *collided_object = initial_game_object;
		while(collided_object != object){
			if(have_same_board_location(collided_object, object)){
				initial_colliding_game_object =
					new_game_object_list_element(collided_object,
							new_game_object_list_element(object,
								initial_colliding_game_object));
				return TRUE;
			}
			collided_object = collided_object->next;
		}
	}
	/* This shouldn't happen, but maybe there was some writing on the screen for
	 * some reason. */
	return FALSE;
}

chtype(*resolve_collision)(struct game_object_list_element *objects) = NULL;

void resolve_collisions(void){
	while(initial_colliding_game_object != NULL){
		register struct game_object_list_element *temp;
		register struct game_object_list_element *first =
			initial_colliding_game_object;
		while(initial_colliding_game_object->next &&
				have_same_board_location(initial_colliding_game_object->object,
					initial_colliding_game_object->next->object)){
			initial_colliding_game_object = initial_colliding_game_object->next;
		}

		/* split the list */
		temp = initial_colliding_game_object;
		initial_colliding_game_object = initial_colliding_game_object->next;
		temp->next = NULL;

		/* resolve the collision and then discard the old list elements */
		if(resolve_collision)
		{
			mvaddch(screen_to_board(first->object->location.y),
				screen_to_board(first->object->location.x),
				resolve_collision(first));
		}
		while(first){
			temp = first->next;
			ditch_game_object_list_element(first);
			first = temp;
		}
	}
}
#endif
