#include "cpuMono.h"
#include "lib.h"

//Agora sim é para salvar as instruções
void store(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter , int *rodando) {
    // Caso precise, acessa a memória 
    if (pontMemoria->acessarMemoria == 1) {          // LOAD lê da memória
        pontMemoria->resultadoOPS = memoria[pontMemoria->enderecoMemoria];

    } else if (pontMemoria->acessarMemoria == 2) {    // STORE escreve na memória
        memoria[pontMemoria->enderecoMemoria] = pontMemoria->dadoParaMemoria;
    }

    // Write-back: grava o resultado no banco de registradores
    if (pontMemoria->escreverRegistrador) {
        bancoReg[pontMemoria->regAlvo] = pontMemoria->resultadoOPS;
    }

    // Atualização do PC nos desvios
    if (pontMemoria->atualizaPC) {
        *programaCounter = pontMemoria->novoPC - 1; // -1 pois cpuStart faz PC++ logo depois
    }

    // Encerramento do programa (syscall serviço 0)
    if (pontMemoria->encerrarPrograma) {
        *rodando = 0;
    }
}