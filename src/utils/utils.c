#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "hash.h"

#define ARROBA '@'
#define NUMERAL '#'

#define TAMANIO_INICIAL_TEXTO 1000

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

// Convierte un entero a string. Fuente: http://newbiegeek.com/convertir-un-entero-a-cadena-de-caracteres-en-c/
static char* itoa(int val, int base) {

	char* zero = "0";
	if ( val == 0) return zero;

	static char buf[32] = {0};
	int i = 30;
	
	for (; val && i ; --i, val /= base) {
		buf[i] = "0123456789abcdef"[val % base];
	}
	
	return &buf[i+1];
}

// Devuelve la poscion en el arreglo de identificadores con el primer false que encuentra.
char* buscar_pos(bool ids[]) {

	int i = 0;
	
	while (i < TAM_INICIAL) {
		
		if (!ids[i]) {
			
			ids[i] = true;
			return itoa(i, 10);
		}
		
		i++;
	}

	return NULL;
}

