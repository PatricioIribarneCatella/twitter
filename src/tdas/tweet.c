#include <stdlib.h>
#include <string.h>

#include "tweet.h"

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

// Destruye los datos alamacenados en el hash de los tweets.
void destruir_dato_tweet(void* dato) {

	tweet_t* tweet = (tweet_t*)dato;
	free(tweet->autor);
	free(tweet->texto);
	free(tweet);
}

