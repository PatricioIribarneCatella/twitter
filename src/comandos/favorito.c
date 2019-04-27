#include <stdio.h>

#include "hash.h"

void favorito(hash_t* tweets, char* id) {

	tweet_t* tweet = hash_obtener(tweets, id);
	
	if (!tweet) {
		
		printf("ERROR_TWEET_ID_INVALIDO\n");
		return;
	}
	
	(tweet->cant_favoritos)++;
	
	printf("OK %s\n", id);
}

