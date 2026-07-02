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
	register int programaCounter = 1;

	// Registradores de proposito geral
	register int r0 = {0};
	register int r1 = {0};
	register int r2 = {0};
	register int r3 = {0};
	register int r4 = {0};
	register int r5 = {0};
	register int r6 = {0};
	register int r7 = {0};

	// criando um vetor, com o proposito de armazenar o binario que sera lido. Tipo uint16_t, armazena apenas numeros positivos, e é inicializado com 0
	uint16_t memoria[65536] = {0};
	load_binary_to_memory(argv[1] , memoria , sizeof(memoria));

	// Ciclos do processador Monociclo
	while(true) {
		// Syscall para acabar
		switch(codigo) {
			// SYSCALL
			case exemploEncerrar
		}
		else {
			// Ciclo Fetch
			// Mexer com o PC 

			// Ciclo Decode
			const uint16_t bitsArquivo = extract_bits(); 	// Salvando os bits extraidos vai de 0 à 65535

			// Ciclo Execute
			switch(codigo) {
				case ADD
				//add ...
			}
			// Ciclo Store
			programaCounter++;
		}
	}
	return 0;
}