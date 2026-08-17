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

    // Encerramento do programa (syscall serviço 0)
    if (pontExecute->encerrarPrograma) {
        *rodando = 0;
    }
}