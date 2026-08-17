#include "lib.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "preditor.h"
#include "instru.h"

uint16_t bancoReg[8] = {0};
uint16_t novoProgramaCounter = 0;

InstrucaoFetch fetchando = {0};
InstrucaoDecode decodando = {0};

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    int houveFlush = 0;

    while(rodando) {

        InstrucaoExecute executando = {0};
        
        int executar = execute(&decodando , &executando , bancoReg , houveFlush);

        store(&executando , bancoReg , memoria , &rodando);

        int decodar = decode(&fetchando , &decodando , (executar != 0));

        uint16_t novoProgramaCounter = 0;
        if(executar != 0){ // vai ter JUMP_COND
            novoProgramaCounter = executar;
        }

        else if (decodar != 0){ // VAI TER JUMP
             novoProgramaCounter = decodar;
        }
        if (novoProgramaCounter != 0) {
            *programaCounter = novoProgramaCounter;
        }

        houveFlush = (novoProgramaCounter != 0);


        fetch(&fetchando , memoria , *programaCounter , houveFlush , novoProgramaCounter);

        (*programaCounter)++;
    }
}
