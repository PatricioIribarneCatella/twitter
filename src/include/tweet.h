#ifndef TWEET_H
#define TWEET_H

#include <stddef.h>
#include <stdlib.h>

typedef struct tweet {
	char* texto;
	char* autor;
	size_t cant_favoritos;
	size_t id;
} tweet_t;

tweet_t* crear_tweet(char* autor, char* texto, size_t id);

void destruir_dato_tweet(void* dato);

#endif // TWEET_H
