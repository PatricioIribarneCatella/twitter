#include <stdlib.h>

#include "lista.h"
#include "hash.h"

// Destruye los datos almacenados en el hash de los usuarios/hashtags.
void destruir_dato_usuario_hashtag(void* dato) {

	lista_destruir(dato, free);
}

// Destruye la linea leída por entrada estandar.
void destruir_linea(lista_t* lista) {

	lista_destruir(lista, free);
}

// Destruye todos los datos almacenados a lo largo de la ejecución del programa.
void destruir_datos(lista_t* linea, hash_t* tweets, hash_t* usuario_hashtag) {

	destruir_linea(linea);
	hash_destruir(tweets);
	hash_destruir(usuario_hashtag);
}

