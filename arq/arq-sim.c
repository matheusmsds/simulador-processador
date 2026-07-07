#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "lib.h"

int main (int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}
	// Registrador PC
	uint16_t programaCounter = 1;

	// criando um vetor, com o proposito de armazenar o binario que sera lido. Tipo uint16_t, armazena apenas numeros positivos, e é inicializado com 0
	uint16_t memoria[65536] = {0};
	load_binary_to_memory(argv[1] , memoria , sizeof(memoria));
	return 0;
}