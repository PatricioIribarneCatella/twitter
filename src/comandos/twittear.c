#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

#include "lista.h"
#include "hash.h"
#include "tweet.h"

void twittear(lista_t* lista, hash_t* tweets, hash_t* usuario_hashtag, bool ids[]) {

	char* autor = lista_borrar_primero(lista);
	
	if (!autor) {
		printf("ERROR_COMANDO_INVALIDO\n");
		return;
	}

	char* clave = buscar_pos(ids);
	char* clave_id = malloc(sizeof(char)*(strlen(clave) + 1));
	strcpy(clave_id, clave);
	
	char* texto = convertir_lista_a_texto(lista, usuario_hashtag, clave_id);
	
	if (texto == NULL) {
		
		free(autor);
		free(clave_id);
		printf("ERROR_TWEET_DEMASIADO_LARGO\n");

	} else {

		if (hash_pertenece(usuario_hashtag, autor)) {
			
			lista_t* listado_tweets = hash_obtener(usuario_hashtag, autor);
			lista_insertar_primero(listado_tweets, clave_id);

		} else {
			
			lista_t* listado_tweets = lista_crear();
			lista_insertar_primero(listado_tweets, clave_id);
			hash_guardar(usuario_hashtag, autor, listado_tweets);
		}

		size_t identificador = (size_t)atoi(clave_id);
		tweet_t* tweet_nuevo = crear_tweet(autor, texto, identificador);
		
		hash_guardar(tweets, clave, tweet_nuevo);

		free(autor);
		printf("OK %d\n", atoi(clave));
	}
}

