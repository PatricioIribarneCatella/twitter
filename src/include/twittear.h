#ifndef TWITTEAR_H
#define TWITTEAR_H

#include <stdbool.h>

#include "lista.h"
#include "hash.h"

void twittear(lista_t* l, hash_t* tweets, hash_t* usr, bool ids[]);

#endif // TWITTEAR_H
