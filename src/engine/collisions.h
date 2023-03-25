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
 * Returns TRUE if a collision was detected, otherwise FALSE.
 */
int detect_collision(struct game_object *object);

/** Resolve all unresolved collisions previously found by detect_collision,
 */
void resolve_collisions(void);

/** Resolve the collision of two or more game objects. The input object of this
 * method is the head of a linked list of involved objects. The order is the
 * same as the order in which the objects are handled, so for any pair of
 * objects whichever comes first in the game_object_list_element list will also
 * appear first in the overarching game_object list. This method shall return
 * the chtype to be rendered at the location of the collision. This method
 * pointer may be set to NULL, in which case no handling is done and the first
 * object to be handled will be rendered.
 */
extern chtype(*resolve_collision)(struct game_object_list_element *objects);
#endif

#endif
