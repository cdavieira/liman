#pragma once

#include "utils/types/common.h"

typedef struct SizedIterator SizedIterator;

SizedIterator* sizedIterator_new(void);

SizedIterator* sizedIterator_destroy(SizedIterator* it);

void sizedIterator_set(SizedIterator* it, void** elements, size_t sz);

void sizedIterator_advance(SizedIterator* it);

void* sizedIterator_get(SizedIterator* it);

int sizedIterator_has_next(SizedIterator* it);

int sizedIterator_is_over(SizedIterator* it);

void sizedIterator_restart(SizedIterator* it);

void sizedIterator_loop(SizedIterator* it, int (*callback)(SizedIterator*, void*), void* data);
