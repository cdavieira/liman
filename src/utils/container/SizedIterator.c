#include "utils/container/SizedIterator.h"
#include "platform/mem.h"

struct SizedIterator {
	size_t sz;
	size_t idx;
	void** elements;
};

SizedIterator* sizedIterator_new(void) {
  SizedIterator* it = mem_zalloc(sizeof(SizedIterator));
  return it;
}

SizedIterator* sizedIterator_destroy(SizedIterator* it) {
	it->elements = NULL;
	it->idx = 0;
	it->sz = 0;
	return mem_free(it);
}

void sizedIterator_set(SizedIterator* it, void** elements, size_t sz) {
	it->elements = elements;
	it->idx = 0;
	it->sz = sz;
}

void sizedIterator_advance(SizedIterator* it) {
	if (!sizedIterator_is_over(it)) {
		it->idx++;
	}
}
void* sizedIterator_get(SizedIterator* it) {
	return !sizedIterator_is_over(it) ? it->elements[it->idx] : NULL;
}

int sizedIterator_has_next(SizedIterator* it) {
	return (it->idx + 1) < it->sz;
}

int sizedIterator_is_over(SizedIterator* it) {
	return it->idx >= it->sz;
}

void sizedIterator_restart(SizedIterator* it) {
	it->idx = 0;
}

void sizedIterator_loop(SizedIterator* it, int (*callback)(SizedIterator*, void*), void* data) {
	while(!sizedIterator_is_over(it)) {
		callback(it, data);
		sizedIterator_advance(it);
	}
	sizedIterator_destroy(it);
}
