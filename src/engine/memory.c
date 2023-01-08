#include <stdlib.h>
#include "memory.h"

static struct game_object_list_element *game_object_list_element_pool = NULL;
static struct game_object *game_object_pool = NULL;

struct game_object *new_game_object(
		unsigned char character, struct game_object *next,
		unsigned short int location_x, unsigned short int location_y,
		short int velocity_x, short int velocity_y){
	struct game_object *return_value;
	if(game_object_pool){
		return_value = game_object_pool;
		game_object_pool = game_object_pool->next;
	}
	else
		return_value = malloc(sizeof(struct game_object));
	return_value->character = character;
	return_value->next = next;
	return_value->location.x = location_x;
	return_value->location.y = location_y;
	return_value->velocity.x = velocity_x;
	return_value->velocity.y = velocity_y;
	return return_value;
}

void ditch_game_object(struct game_object *object){
	/* TODO: add a "safe" mode that checks for null, and maybe a diagnostics
	 * mode too? */
	object->next = game_object_pool;
	game_object_pool = object;
}

struct game_object_list_element *new_game_object_list_element(
		struct game_object *object, struct game_object_list_element *next){
	struct game_object_list_element *return_value;
	if(game_object_list_element_pool){
		return_value = game_object_list_element_pool;
		game_object_list_element_pool = game_object_list_element_pool->next;
	}
	else
		return_value = malloc(sizeof(struct game_object_list_element));
	return_value->object = object;
	return_value->next = next;
	return return_value;
}

void ditch_game_object_list_element(struct game_object_list_element *element){
	element->next = game_object_list_element_pool;
	game_object_list_element_pool = element;
}
