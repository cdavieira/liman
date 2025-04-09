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
static Element* cl_get_element(CircularList* lc, size_t index);

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

CircularList* cl_add(CircularList* lc, void* item, size_t index){ //lc desordenada
	Element *elem = element_new(item, 0, 0);
	if(lc->n){
		//non empty list
		if(index >= lc->n){
			//add as the last element
			elem->prev = lc->begin->prev;
			elem->next = lc->begin;
			lc->begin->prev->next = elem;
			lc->begin->prev = elem;
		}
		else{
			Element *old = cl_get_element(lc, index);
			elem->next = old;
			elem->prev = old->prev;
			elem->prev->next = elem;
			old->prev = elem;
			if(!index){
				lc->begin = elem;
			}
		}
	}
	else{
		//empty list, add as the first element
		elem->next = elem;
		elem->prev = elem;
		lc->begin = elem;
	}
	lc->n++;
	return lc;
}

CircularList* cl_add_inorder(CircularList* lc, void* item, int (*fcmp)(void*, void*)){
	void* cmp_item = NULL;
	size_t index = lc->n;
	const size_t sz = lc->n;
	for(size_t i=0; i<sz; i++){
		cmp_item = cl_get_item(lc, i);
		if(!fcmp(item, cmp_item)){
			index = i;
			break;
		}
	}
	lc = cl_add(lc, item, index);
	return lc;
}

void* cl_remove(CircularList* lc, size_t index){
	if(!lc->n){
		return NULL;
	}

	index %= lc->n;
	Element* removed = cl_get_element(lc, index);
	void* item = removed->item;

	if(removed == lc->begin){
		lc->begin = lc->n == 1 ? NULL : removed->next;
	}
	removed->prev->next = removed->next;
	removed->next->prev = removed->prev;
	free(removed);
	lc->n--;

	return item;
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
