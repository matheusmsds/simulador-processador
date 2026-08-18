#include "lib.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "preditor.h"
#include "instru.h"

uint16_t bancoReg[8] = {0};
uint16_t novoProgramaCounter = 0;                       // vai servir para passar o novo endereco para programaCounter, após um JUMP ou JUMP_COND

InstrucaoFetch fetchando = {0};
InstrucaoDecode decodando = {0};

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    int houveFlush = 0;                                 // indica se as instrucoes devem ser descartadas ou não

    while(rodando) {

        InstrucaoExecute executando = {0};

        // O input de uma fase é o output para outra fase
        
        int executar = execute(&decodando , &executando , bancoReg , houveFlush);

        store(&executando , bancoReg , memoria , &rodando);

        int decodar = decode(&fetchando , &decodando , (executar != 0));

        uint16_t novoProgramaCounter = 0;
        if(executar != 0){ // vai ter JUMP_COND
            novoProgramaCounter = executar;             // passando o valor do desvio para o novoProgramaCounter
        }

        else if (decodar != 0){ // VAI TER JUMP
             novoProgramaCounter = decodar;             // passando o valor do desvio para o novoProgramaCounter
        }
        if (novoProgramaCounter != 0) {     
            *programaCounter = novoProgramaCounter;     // alterando o valor de programaCounter
        }

        houveFlush = (novoProgramaCounter != 0);        // lógica para saber se houve flush


        fetch(&fetchando , memoria , *programaCounter , houveFlush , novoProgramaCounter);

        if (fetchando.enderecoPrevisto != 0) {          // endereco previsto, se tem um endereco previsto para desvio, vai ser desviado
            *programaCounter = fetchando.enderecoPrevisto;      
        } else {                                        // caso contrário, segue normalmente o programa
            (*programaCounter)++;
        }
    }
}