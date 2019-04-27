#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "lista.h"
#include "heap.h"

#define CRONOLOGICO "cronologico"
#define POPULAR "popular"

// Función de comparación.
int cmp(const void *a, const void *b) {

	tweet_t* c = (tweet_t*)a;
	tweet_t* d = (tweet_t*)b;

	if (c->cant_favoritos < d->cant_favoritos) {
		return 1;
	} else if (c->cant_favoritos > d->cant_favoritos) {
		return -1;
	} else {
		return 0;
	}
}

void buscar(hash_t* tweets, hash_t* usuario_hashtag, char* autor_hashtag, char* orden, char* cantidad) {

	/* Verifica que el orden exista */
	if (orden == NULL) {
		printf("ERROR_COMANDO_INVALIDO\n");
		return;
	}

	/* Verifica que la cantidad exista */
	if (cantidad == NULL) {
		printf("ERROR_COMANDO_INVALIDO\n");
		return;
	}

	/* Verifica que el orden sea el correcto: CRONOLOGICO o POPULAR*/
	if (strcmp(orden, CRONOLOGICO) != 0 && strcmp(orden, POPULAR) != 0) {
		printf("ERROR_COMANDO_INVALIDO\n");
		return;
	}

	/* Verifica que el usuario o hastag por el cual se quiere realizar la busqueda exista */
	lista_t* ids = hash_obtener(usuario_hashtag, autor_hashtag);
	
	if (ids == NULL) {
		printf("OK 0\n");
		return;
	}
	
	lista_iter_t* iter = lista_iter_crear(ids);

	if (strcmp(orden, CRONOLOGICO) == 0) {

		if (atoi(cantidad) == 0) {
			/* Primer caso: se pide mostrar toda la informacion almacenada */

			printf("OK %d\n", (int)lista_largo(ids));
			
			while (!lista_iter_al_final(iter)) {
				
				char* clave_id = lista_iter_ver_actual(iter);
				tweet_t* tweet = hash_obtener(tweets, clave_id);
				printf("%d %s %s\n", (int)tweet->id, tweet->autor, tweet->texto);
				lista_iter_avanzar(iter);
			}
			
			lista_iter_destruir(iter);
			return;

		} else {
			/* Segundo caso: se pide mostrar parte de la informacion almacenada */

			int i = 0;
			
			if (atoi(cantidad) > (int)lista_largo(ids)) {
				
				printf("OK %d\n", (int)lista_largo(ids));

			} else {
				
				printf("OK %s\n", cantidad);
			}
			
			while (i < atoi(cantidad) && !lista_iter_al_final(iter)) {
				
				char* clave_id = lista_iter_ver_actual(iter);
				
				tweet_t* tweet = hash_obtener(tweets, clave_id);
				
				printf("%d %s %s\n", (int)tweet->id, tweet->autor, tweet->texto);
				
				lista_iter_avanzar(iter);
				i++;
			
			}
			
			lista_iter_destruir(iter);
			return;
		}

	} else if (strcmp(orden, POPULAR) == 0) {

		if (atoi(cantidad) == 0 || atoi(cantidad) > (int)lista_largo(ids)) {
			/* Primer caso: se pide mostrar toda la informacion almacenada o si la cantidad pedida supera la existente */

			heap_t* top_cantidad = heap_crear(cmp, lista_largo(ids));
			
			while (!lista_iter_al_final(iter)) {
				
				char* clave_id = lista_iter_ver_actual(iter);
				tweet_t* tweet = hash_obtener(tweets, clave_id);
				heap_encolar(top_cantidad, tweet);
				lista_iter_avanzar(iter);
			}
			
			lista_iter_destruir(iter);

			/* Los desencola del heap y los inserta en una lista porque el heap es de minimos y tienen que salir en oreden decreciente*/
			lista_t* lista_aux = lista_crear();
			
			while (!heap_esta_vacio(top_cantidad)) {
				lista_insertar_primero(lista_aux, heap_desencolar(top_cantidad));
			}

			printf("OK %d\n", (int)lista_largo(ids));
			
			while (!lista_esta_vacia(lista_aux)) {
				
				tweet_t* tweet_aux = lista_borrar_primero(lista_aux);
				printf("%d %s %s | Nivel de Popularidad %d\n", (int)tweet_aux->id, tweet_aux->autor, tweet_aux->texto, (int)tweet_aux->cant_favoritos);
			}
			
			lista_destruir(lista_aux, NULL);
			heap_destruir(top_cantidad, NULL);
			return;

		} else {
			/* Segundo caso: se pide mostrar parte de la informacion almacenada */

			int i = 0;
			heap_t* top_cantidad = heap_crear(cmp, (size_t)atoi(cantidad));

			/* Encola en el heap hasta que la cantidad sea la especificada */
			while (i < atoi(cantidad)) {
				
				char* clave_id = lista_iter_ver_actual(iter);
				tweet_t* tweet = tweet = hash_obtener(tweets, clave_id);
				heap_encolar(top_cantidad, tweet);
				lista_iter_avanzar(iter);
				i++;
			}

			/* Sigue iterando la lista y compara cada elemento con el tope del heap.
				Si el elemento es mayor que el tope, se desencola el tope y se guarda el elemento.
				Sino se descarta.
			*/
			while (!lista_iter_al_final(iter)) {
				
				char* clave_id = lista_iter_ver_actual(iter);
				tweet_t* tweet = tweet = hash_obtener(tweets, clave_id);
				
				if (cmp(heap_ver_max(top_cantidad), tweet) > 0) {
					
					heap_remover(top_cantidad);
					heap_encolar(top_cantidad, tweet);
				}
				
				lista_iter_avanzar(iter);
			}

			lista_iter_destruir(iter);

			/* Los desencola del heap y los inserta en una lista porque el heap es de minimos y tienen que salir en oreden decreciente*/
			lista_t* lista_aux = lista_crear();
			
			while (!heap_esta_vacio(top_cantidad)) {
				lista_insertar_primero(lista_aux, heap_desencolar(top_cantidad));
			}

			printf("OK %s\n", cantidad);
			
			while (!lista_esta_vacia(lista_aux)) {
				
				tweet_t* tweet_aux = lista_borrar_primero(lista_aux);
				printf("%d %s %s | Nivel de Popularidad %d\n", (int)tweet_aux->id, tweet_aux->autor, tweet_aux->texto, (int)tweet_aux->cant_favoritos);
			}
			
			lista_destruir(lista_aux, destruir_dato_tweet);
			heap_destruir(top_cantidad, destruir_dato_tweet);
			return;
		}
	}
}

