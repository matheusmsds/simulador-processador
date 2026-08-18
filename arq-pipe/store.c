#include <stdio.h>
#include "execute.h"

//Agora sim é para salvar as instruções
void store(InstrucaoExecute *pontExecute , uint16_t bancoReg[] , uint16_t memoria[] , int *rodando) {
    // Caso precise, acessa a memória 
    if (pontExecute->acessarMemoria == 1) {          // LOAD lê da memória
        pontExecute->resultadoOPS = memoria[pontExecute->enderecoMemoria];

    } else if (pontExecute->acessarMemoria == 2) {    // STORE escreve na memória
        memoria[pontExecute->enderecoMemoria] = pontExecute->dadoParaMemoria;
    }

    // Write-back: grava o resultado no banco de registradores
    if (pontExecute->escreverRegistrador) {
        bancoReg[pontExecute->regAlvo] = pontExecute->resultadoOPS;
    }

    // Dispatch dos serviços de syscall (definidos pelo valor de r0)
    if (pontExecute->encerrarPrograma) {
        switch (bancoReg[0]) {
            case 0: // encerra o programa
                *rodando = 0;
                break;

            case 1: { // imprime string: r1 = endereço, terminada em word 0
                uint16_t endereco = bancoReg[1];
                while (memoria[endereco] != 0) {
                    putchar((char) memoria[endereco]);
                    endereco++;
                }
                break;
            }

            case 2: // imprime nova linha
                putchar('\n');
                break;

            case 3: // imprime inteiro: r1 = valor
                printf("%u", bancoReg[1]);
                break;

            default:
                break; // serviço não implementado
        }
    }
}