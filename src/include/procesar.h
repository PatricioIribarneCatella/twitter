#ifndef PROCESAR_H
#define PROCESAR_H

#include <stdbool.h>

#include "lista.h"
#include "hash.h"

void procesar_linea(lista_t *l, hash_t* tweets, hash_t* usr, bool ids[]);

#endif // PROCESAR_H
