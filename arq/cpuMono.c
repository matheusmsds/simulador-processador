#include "cpuMono.h"
#include "lib.h"

uint16_t bancoReg[8] = {0};

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    while(rodando) {
        uint16_t instrucao = fetch(memoria , *programaCounter);

        InstrucaoDecodificada instrucaoDecodificada = {0};

        decode(instrucao , &instrucaoDecodificada);

        execute(&instrucaoDecodificada , bancoReg);

        store(&instrucaoDecodificada , bancoReg , memoria , programaCounter , &rodando);

        (*programaCounter)++;
    }
}