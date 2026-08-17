#include "fetch.h"
#include "preditor.h"
#include <stdio.h>

void fetch(InstrucaoFetch *pontFetch , uint16_t memoria[] , uint16_t programaCounter , int houveFlush , uint16_t novoProgramaCounter) {
    if(houveFlush){                                                         // indica se teve flush, caso tenha ---> bolha, caso não ---> decode normalmente
        pontFetch->instrucao = memoria[novoProgramaCounter];
        pontFetch->pc = novoProgramaCounter;
        pontFetch->enderecoPrevisto = 0;

    }
    else {
        int previsaoEndereco = previsao(programaCounter);
        if(previsaoEndereco != 0){
            pontFetch->instrucao = memoria[previsaoEndereco];
            pontFetch->pc = previsaoEndereco;
            pontFetch->enderecoPrevisto = previsaoEndereco;

        } else {
        pontFetch->instrucao = memoria[programaCounter];
        pontFetch->pc = programaCounter;
        pontFetch->enderecoPrevisto = 0;
        }
    }
    pontFetch->temInstrucao = 1;
    printf("DEBUG fetch: pc=%u instrucao=%u houveFlush_usado=%d\n", pontFetch->pc, pontFetch->instrucao, houveFlush);
}