#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"
#include "heap.h"
#include "lista.h"

#define TWITTEAR "twittear"
#define FAVORITO "favorito"
#define BUSCAR "buscar"
#define CRONOLOGICO "cronologico"
#define POPULAR "popular"

#define FIN_LINEA '\n'
#define FIN_STRING '\0'
#define ESPACIO ' '
#define ARROBA '@'
#define NUMERAL '#'

#define TAMANIO_INICIAL_TEXTO 1000
#define TAMANIO_INICIAL_BUFFER 10
#define TAM_INICIAL 1000

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct tweet {
	char* texto;
	char* autor;
	size_t cant_favoritos;
	size_t id;
} tweet_t;

/* ******************************************************************
 *                FUNCION DE LECTURA DE COMANDOS
 * *****************************************************************/

lista_t* leer_linea() {

	char c;
	size_t tam = TAMANIO_INICIAL_BUFFER;
	int i;

	lista_t *lista = lista_crear();

	if (!lista) return NULL;

	c = getchar();

	while (c != FIN_LINEA && c != EOF) {

		char* buffer = malloc(sizeof(char)*(tam));
		
		if (!buffer) return NULL;
		
		i = 0;
		
		while (c != ESPACIO && c != FIN_LINEA) {

			if (i + 1 == tam) {
				
				tam += tam;
				char* aux = realloc(buffer, tam * sizeof(char));
				if (!aux) return NULL;
				buffer = aux;
			}
			
			buffer[i] = c;
			c = getchar();
			i++;
		}
		
		buffer[i] = FIN_STRING;
		lista_insertar_ultimo(lista, buffer);
		
		if (c != FIN_LINEA) c = getchar();
	}

	return lista;
}

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

// Crea un nuevo tweet.
tweet_t* crear_tweet(char* autor, char* texto, size_t id) {

	tweet_t* tweet = malloc(sizeof(tweet_t));

	if (!tweet) return NULL;

	tweet->autor = malloc(sizeof(char)*(strlen(autor) + 1));
	
	if (!tweet->autor) {
		
		free(tweet);
		return NULL;
	}
	
	strcpy(tweet->autor, autor);

	tweet->texto = texto;

	tweet->id = id;
	tweet->cant_favoritos = 0;
	
	return tweet;
}

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

// Convierte un entero a string. Fuente: http://newbiegeek.com/convertir-un-entero-a-cadena-de-caracteres-en-c/
char* itoa(int val, int base) {

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

// Destruye los datos alamacenados en el hash de los tweets.
void destruir_dato_tweet(void* dato) {

	tweet_t* tweet = (tweet_t*)dato;
	free(tweet->autor);
	free(tweet->texto);
	free(tweet);
}

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

/* ******************************************************************
 *                TWITTEAR       FAVORITO        BUSCAR
 * *****************************************************************/

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

void favorito(hash_t* tweets, char* id) {

	tweet_t* tweet = hash_obtener(tweets, id);
	
	if (!tweet) {
		
		printf("ERROR_TWEET_ID_INVALIDO\n");
		return;
	}
	
	(tweet->cant_favoritos)++;
	
	printf("OK %s\n", id);
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

/* ******************************************************************
 *                         PROCESAR LINEA
 * *****************************************************************/

void procesar_linea(lista_t *lista, hash_t* tweets, hash_t* usuario_hashtag, bool ids[]) {

	if (!lista_esta_vacia(lista)) {
		
		char *comando = lista_borrar_primero(lista);

		if (strcmp(comando, TWITTEAR) == 0) {
			
			twittear(lista, tweets, usuario_hashtag, ids);

		} else if (strcmp(comando, FAVORITO) == 0) {
			
			char* id = lista_borrar_primero(lista);
			
			if (!id) {
				
				printf("ERROR_COMANDO_INVALIDO\n");
				free(comando);
				return;
			}
			
			if (!lista_esta_vacia(lista)) {
				
				printf("ERROR_COMANDO_INVALIDO\n");
				free(id);
				free(comando);
				return;

			} else {

				favorito(tweets, id);
				free(id);
			}

		} else if (strcmp(comando, BUSCAR) == 0) {
			
			char* autor_hashtag = lista_borrar_primero(lista);
			char* orden = lista_borrar_primero(lista);
			char* cantidad = lista_borrar_primero(lista);
			
			buscar(tweets, usuario_hashtag, autor_hashtag, orden, cantidad);
			
			free(autor_hashtag);
			free(orden);
			free(cantidad);

		} else {
			
			printf("ERROR_COMANDO_INVALIDO\n");
		}

		free(comando);
	}
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
