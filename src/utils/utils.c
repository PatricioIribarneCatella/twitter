
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

