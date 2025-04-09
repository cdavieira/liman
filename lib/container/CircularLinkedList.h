#pragma once

#include <stddef.h>

typedef struct CircularList CircularList;

CircularList* cl_new(void);
CircularList* cl_destroy(CircularList* l, void* (*free_element)(void*));

size_t cl_get_size(CircularList* l);
void* cl_get_item(CircularList* l, size_t idx);

/** add 'item' to the list, putting it at index 'idx'
 *
 * if 'idx' is greater than the current size of the circular list, then the
 * item is effectively appended, thus becoming on the right of all remaining
 * items of the list.
 *
 * if 'idx' is lesser or equal to the current size of the list, then the item
 * gets positioned at 'idx' and the element previously occupying that idx goes to
 * its right.
 * */
CircularList* cl_add(CircularList* l, void* item, size_t idx);

/**
 * add an item somewhere in the list, so that it remains sorted
 */
CircularList* cl_add_inorder(CircularList* l, void* item, int (*fcmp)(void*, void*));

/** remove the item stored at index 'idx' */
void* cl_remove(CircularList* l, size_t idx);
