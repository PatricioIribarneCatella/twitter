#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"
#include "heap.h"
#include "lista.h"


#define ARROBA '@'
#define NUMERAL '#'

#define TAMANIO_INICIAL_TEXTO 1000
#define TAM_INICIAL 1000


// Función de comparación.
int cmp(const void *a, const void *b) {

	tweet_t* c = (tweet_t*)a;
	tweet_t* d = (tweet_t*)b;

	if (c->cant_favoritos < d->cant_favoritos) {
		return 1;
	}else if (c->cant_favoritos > d->cant_favoritos) {
		return -1;
	}else{
		return 0;
	}
}

// Convierte a texto el tweet ingresado por comando y se fija si hay menciones a usuarios o a hashtags.
char* convertir_lista_a_texto(lista_t* lista, hash_t* usuario_hashtag, char* id) {

	char* texto = malloc(sizeof(char)*TAMANIO_INICIAL_TEXTO);
	
	if (!texto) return NULL;
	
	lista_t* menciones = lista_crear();
	char* palabra = lista_borrar_primero(lista);
	
	strcpy(texto, palabra);
	strcat(texto, " ");

	/* Aca se fija si la primer palabra que saca contiene menciones o no a otros usuarios o hashtags*/
	if (palabra[0] == ARROBA || palabra[0] == NUMERAL) {
		
		lista_insertar_primero(menciones, palabra);

	} else {
		
		free(palabra);
	}

	/* Itera sobre la lista y sigue sacando palabras y se fija si contienen menciones a usuarios o a hashtags*/
	while (!lista_esta_vacia(lista)) {
		
		palabra = lista_borrar_primero(lista);
		strcat(texto, palabra);
		strcat(texto, " ");
		
		if (palabra[0] == ARROBA || palabra[0] == NUMERAL) {
			
			lista_insertar_primero(menciones, palabra);

		} else {
			
			free(palabra);
		}
	}

	/* Verifica cuan largo es el texto ingresado y de ser mayor a 150 lo descarta y libera memoria pedida anteriormente sino
		redimensiona hasta la longitud del texto ingresado
	*/
	if (strlen(texto) > 150) {
		
		free(texto);
		lista_destruir(menciones, free);
		
		return NULL;

	} else {
		
		char* texto_nuevo = realloc(texto, (strlen(texto) +1)*sizeof(char));
		
		if (!texto_nuevo) {
			free(texto);
			lista_destruir(menciones, free);
		}
		
		texto = texto_nuevo;
	}

	/* Como el texto fue válido, itera las lista de las menciones a usuarios o hastags y las almacena si no existian o
		guardan el id del tweet en la lista asociada a cada usuario o hastag
	*/
	while (!lista_esta_vacia(menciones)) {

		char* mencion = lista_borrar_primero(menciones);
		
		if (hash_pertenece(usuario_hashtag, mencion)) {
			
			lista_t* listado_tweets = hash_obtener(usuario_hashtag, mencion);
			char* clave = malloc(sizeof(char)*(strlen(id)+1));
			strcpy(clave, id);
			
			lista_insertar_primero(listado_tweets, clave);

		} else {
			
			lista_t* listado_tweets = lista_crear();
			char* clave = malloc(sizeof(char)*(strlen(id)+1));
			strcpy(clave, id);
			
			lista_insertar_primero(listado_tweets, clave);
			
			hash_guardar(usuario_hashtag, mencion, listado_tweets);
		}
		
		free(mencion);
	}

	lista_destruir(menciones, free);
	
	return texto;
}




/* ******************************************************************
 *                              TP2
 * *****************************************************************/

void tp2() {

	lista_t *linea = leer_linea();

	if (!linea) {
		
		printf("ERROR INESPERADO DE MEMORIA\n");
		destruir_linea(linea);
		return;
	}

	hash_t* tweets = hash_crear(destruir_dato_tweet);
	hash_t* usuario_hashtag = hash_crear(destruir_dato_usuario_hashtag);
	bool *ids = malloc(sizeof(bool)*TAM_INICIAL);

	for (int i = 0; i < TAM_INICIAL; i++) {
		ids[i] = false;
	}

	while (!lista_esta_vacia(linea)) {

		procesar_linea(linea, tweets, usuario_hashtag, ids);
		destruir_linea(linea);
		linea = leer_linea();

	}

	free(ids);
	destruir_datos(linea, tweets, usuario_hashtag);

}

int main(void) {
	
	tp2();
	return 0;
}
