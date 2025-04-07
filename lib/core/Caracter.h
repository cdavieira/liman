#pragma once


typedef struct Caracter Caracter;
typedef struct bitmap bitmap;


Caracter* caracter_new(unsigned const letra, unsigned long const peso);
Caracter* caracter_destroy(Caracter* c);


unsigned caracter_get_ASCII(Caracter const* const c);
unsigned* caracter_get_ASCII_addr(Caracter *c);
unsigned long caracter_get_weight(Caracter const* const c);
bitmap* caracter_get_bmap(Caracter const* const c);


Caracter* caracter_set_ASCII(Caracter * const c, unsigned const letra);
Caracter* caracter_set_weight(Caracter* const c, unsigned long const peso);
Caracter* caracter_set_bmap(Caracter* const c, bitmap *const bm);


unsigned caracter_compare_ASCII(Caracter const* const c, unsigned const* const letra);
unsigned caracter_compare_weight(Caracter const* const c1, Caracter const* const c2);
