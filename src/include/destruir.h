#ifndef DESTRUIR_H
#define DESTRUIR_H

#include "lista.h"
#include "hash.h"

void destruir_dato_usuario_hashtag(void* dato);
void destruir_linea(lista_t* lista);
void destruir_datos(lista_t* linea, hash_t* tweets, hash_t* usuario_hashtag);

#endif // DESTRUIR_H
