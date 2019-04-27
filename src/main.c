#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"
#include "lectura.h"
#include "procesar.h"
#include "destruir.h"

#include "hash.h"
#include "lista.h"
#include "tweet.h"

/* ******************************************************************
 *                              Twitter
 * *****************************************************************/

void twitter() {

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
	
	twitter();

	return 0;
}

