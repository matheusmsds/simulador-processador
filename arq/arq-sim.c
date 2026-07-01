#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "lib.h"

int main (int argc, char **argv)
{
	// Registrador PC
	register int programaCounter = 1;

	// Registradores de proposito geral
	register int r0;
	register int r1;
	register int r2;
	register int r3;
	register int r4;
	register int r5;
	register int r6;
	register int r7;

	const uint16_t bitsArquivo = extract_bits(); 	// Salvando os bits extraidos vai de 0 à 65535
	load_binary_to_memory();

	// Ciclos do processador Monociclo
	while(true)
	{
		// Syscall para acabar
		if(r0 = 0)
		{
			exit(0);
		}
		else
		{
			// Ciclo Fetch
			while(true)
			{
				//malloc - usamos para reservar a quantidade de memória
				malloc();
			}
			// Ciclo Decode
			while(true)
			{

			}

			// Ciclo Execute
			while(true)
			{

			}

			// Ciclo Store
			while(true)
			{

			}á uma instrução jump para o 
			programaCounter++;
		}
		if (argc != 2) {
			printf("usage: %s [bin_name]\n", argv[0]);
			exit(1);
		}
		else{
			printf("executou");
		}
		return 0;
		}
}