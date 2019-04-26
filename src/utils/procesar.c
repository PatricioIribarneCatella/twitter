#define TWITTEAR "twittear"
#define FAVORITO "favorito"
#define BUSCAR "buscar"

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

