#ifndef COLLISIONS_INCLUDED
#define COLLISIONS_INCLUDED
#include "engine.h"

#if COLLISION_DETECTION

/** Find the collision object by location, first in collision list than in all
 * objects. When found, both that object and this get added to list. If found in
 * collision list, insert after last one with this location, so that when the
 * logic to collide is called, they are in the order of handling. That way the
 * first item in the list, often the ship or other important stuff like
 * obsticals can easily be printed in the correct order. If it's found in the
 * list of all objects both are prepended to the list of colliding objects, with
 * the more important one appearing first.
 */
int detect_collision(struct game_object *object);

/** TODO: add docs */
void resolve_collisions(void);

/** TODO: add docs */
extern chtype(*resolve_collision)(struct game_object_list_element *objects);
#endif

#endif
