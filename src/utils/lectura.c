#include <stdlib.h>

#include "lista.h"

#define FIN_LINEA '\n'
#define FIN_STRING '\0'
#define ESPACIO ' '

#define TAMANIO_INICIAL_BUFFER 10

/* ******************************************************************
 *                FUNCION DE LECTURA DE COMANDOS
 * *****************************************************************/

lista_t* leer_linea() {

	char c;
	size_t tam = TAMANIO_INICIAL_BUFFER;
	int i;

	lista_t *lista = lista_crear();

	if (!lista)
		return NULL;

	c = getchar();

	while (c != FIN_LINEA && c != EOF) {

		char* buffer = malloc(sizeof(char)*(tam));
		
		if (!buffer)
			return NULL;
		
		i = 0;
		
		while (c != ESPACIO && c != FIN_LINEA) {

			if (i + 1 == tam) {
				
				tam += tam;
				char* aux = realloc(buffer, tam * sizeof(char));
				
				if (!aux)
					return NULL;
				
				buffer = aux;
			}
			
			buffer[i] = c;
			c = getchar();
			i++;
		}
		
		buffer[i] = FIN_STRING;
		lista_insertar_ultimo(lista, buffer);
		
		if (c != FIN_LINEA)
			c = getchar();
	}

	return lista;
}

