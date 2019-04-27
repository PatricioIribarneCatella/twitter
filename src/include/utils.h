#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "lista.h"
#include "hash.h"

char* convertir_lista_a_texto(lista_t* lista, hash_t* usuario_hashtag, char* id);
char* buscar_pos(bool ids[]);

#endif // UTILS_H
