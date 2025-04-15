#include "container/CircularLinkedList.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Element Element;

struct Element{
	void* item;
	Element* next;
	Element* prev;
};

struct CircularList{
	Element* begin;
	size_t n;
};

static Element* element_new(void* item, Element* next, Element* prev);
static Element* element_destroy(Element* elem, void* (*free_item)(void*));
static inline void element_connect(Element* first, Element* second){
	// first <-> second
	first->next = second;
	second->prev = first;
}
static Element* cl_get_element(CircularList* lc, size_t index);

/*
 * the family of unsafe_* functions provides ways of modifying the internals
 * of the linkedlist without any sort of check if that would invalidate the
 * structure of the container.
 *
 * Since they can potentially corrupt the container state, they were named 'unsafe'
 *
 * These shouldn't be used where they aren't supposed to, that is, the checks
 * themselves should be done by the callee before calling any of
 * these functions
 *
 * Another way to think about them is that they become safe functions when the
 * conditions that they were thought to operate over are satisfied
 *
 * Why have such functions? to avoid unnecessary conditionals in parts of the
 * code and to reuse the same routine throughtout this translation unit
 *
 * Conditions which should be true when using these functions:
 *
 * add_first: l->s == 0
 * add_start: l->s != 0 and adding an elem at idx == 0
 * add_end:   l->s != 0 and adding an elem at idx >= l->n
 *
 * rm_first: l->s == 1
 * rm_start: l->s > 1 and removing an elem at idx == 0
 * rm_end:   l->s > 1 and removing an elem at idx >= l->n
*/
static CircularList* cl_unsafe_add_first(CircularList* l, Element* e);
static CircularList* cl_unsafe_add_start(CircularList* l, Element* e);
static CircularList* cl_unsafe_add_end(CircularList* l, Element* e);
static void* cl_unsafe_rm_first(CircularList* l);
static void* cl_unsafe_rm_start(CircularList* l);
static void* cl_unsafe_rm_end(CircularList* l);

CircularList* cl_new(void){
	CircularList* lc = calloc(1, sizeof(CircularList));
	return lc;
}

CircularList* cl_destroy(CircularList* l, void* (*free_element)(void*)){
	Element* elem = NULL;
	Element* next = l->begin;
	while((elem = next)){
		next = elem->next;
		element_destroy(elem, free_element);
	}
	l->begin = NULL; //i'm a good boi
	free(l);
	return NULL;
}





size_t cl_get_size(CircularList* lc){
	return lc ? lc->n : 0;
}

void* cl_get_item(CircularList* lc, size_t index){
	return cl_get_element(lc, index)->item;
}





CircularList* cl_add(CircularList* l, void* item, size_t index){ //l desordenada
	Element *e = element_new(item, 0, 0);
	if(l->n == 0){
		return cl_unsafe_add_first(l, e);
	}
	if(index == 0){
		return cl_unsafe_add_start(l, e);
	}
	if(index >= l->n){
		return cl_unsafe_add_end(l, e);
	}
	Element *old = cl_get_element(l, index);
	// ... <-> old->prev <-> old <-> ....
	element_connect(old->prev, e);
	element_connect(e, old);
	// ... <-> old->prev <-> e <-> old <-> ....
	l->n++;
	return l;
}

CircularList* cl_append(CircularList* l, void* item){
	Element *e = element_new(item, 0, 0);
	if(l->n == 0){
		return cl_unsafe_add_first(l, e);
	}
	return cl_unsafe_add_end(l, e);
}

CircularList* cl_shift(CircularList* l, void* item){
	Element *e = element_new(item, 0, 0);
	if(l->n == 0){
		return cl_unsafe_add_first(l, e);
	}
	return cl_unsafe_add_start(l, e);
}

CircularList* cl_add_inorder(CircularList* lc, void* item, int (*fcmp)(void*, void*)){
	const size_t sz = lc->n;
	for(size_t i=0; i<sz; i++){
		if(!fcmp(item, cl_get_item(lc, i))){
			return cl_add(lc, item, i);
		}
	}
	return cl_append(lc, item);
}





void* cl_remove(CircularList* l, size_t index){
	if(l->n == 0){
		return NULL;
	}
	if(l->n == 1){
		return cl_unsafe_rm_first(l);
	}
	if(index == 0){
		return cl_unsafe_rm_start(l);
	}
	if(index >= l->n){
		return cl_unsafe_rm_end(l);
	}
	Element* removed = cl_get_element(l, index);
	void* item = removed->item;
	// ... <-> removed->prev <-> removed <-> removed->next <-> ....
	element_connect(removed->prev, removed->next);
	// ... <-> removed->prev <-> removed->next <-> ....
	free(removed);
	l->n--;
	return item;
}

void* cl_pop(CircularList* l){
	if(l->n == 0){
		return NULL;
	}
	if(l->n == 1){
		return cl_unsafe_rm_first(l);
	}
	return cl_unsafe_rm_end(l);
}

void* cl_unshift(CircularList* l){
	if(l->n == 0){
		return NULL;
	}
	if(l->n == 1){
		return cl_unsafe_rm_first(l);
	}
	return cl_unsafe_rm_start(l);
}





static Element* element_new(void* item, Element* next, Element* prev){
	Element* elem = calloc(1, sizeof(Element));
	*elem = (Element) {
	  .item = item,
	  .next = next,
	  .prev = prev
	};
	return elem;
}

static Element* element_destroy(Element* elem, void* (*free_item)(void*)){
	if(!elem){
		return NULL;
	}
	if(elem->item){
		free_item(elem->item);
	}
	free(elem);
	return NULL;
}

static Element* cl_get_element(CircularList* lc, size_t index){
	Element* elem = lc->begin;
	index %= lc->n;
	for(size_t i=0; elem && i<index; i++){
		elem = elem->next;
	}
	return elem;
}

static CircularList* cl_unsafe_add_first(CircularList* l, Element* e){
	// NULL
	element_connect(e, e);
	//... <-> e <-> e <-> e <-> ...

	l->begin = e;
	l->n = 1;
	return l;
}

static CircularList* cl_unsafe_add_start(CircularList* l, Element* e){
	//l->begin->prev <-> l->begin <-> l->begin->next
	element_connect(l->begin->prev, e);
	element_connect(e, l->begin);
	//l->begin->prev <-> e <-> l->begin <-> l->begin->next

	l->begin = e;
	l->n++;
	return l;
}

static CircularList* cl_unsafe_add_end(CircularList* l, Element* e){
	//l->begin->prev <-> l->begin <-> l->begin->next
	element_connect(l->begin->prev, e);
	element_connect(e, l->begin);
	//l->begin->prev <-> e <-> l->begin <-> l->begin->next

	l->n++;
	return l;
}

static void* cl_unsafe_rm_first(CircularList* l){
	void* item = l->begin->item;
	free(l->begin);
	l->begin = NULL;
	l->n = 0;
	return item;
}

static void* cl_unsafe_rm_start(CircularList* l){
	Element* first = l->begin;
	void* item = first->item;

	// first->prev <-> first <-> first->next
	element_connect(first->prev, first->next);
	l->begin = first->next;
	// first->prev <-> first->next

	free(first);
	l->n--;
	return item;
}

static void* cl_unsafe_rm_end(CircularList* l){
	Element* last = l->begin->prev;
	void* item = last->item;

	// last->prev <-> last <-> last->next
	element_connect(last->prev, last->next);
	// last->prev <-> last->next

	free(last);
	l->n--;
	return item;
}
