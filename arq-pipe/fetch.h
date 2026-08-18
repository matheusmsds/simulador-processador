#ifndef FETCH_H
#define FETCH_H

#include <stdint.h>

extern uint16_t bancoReg[8];

typedef struct
{
    uint16_t  pc;                       // Endereço que foi fetchado
    uint16_t  instrucao;                // Instrucao do pc
    int temInstrucao;                   // Indica se a instrução do pc é válida
    uint16_t enderecoPrevisto;          // Previsão do endereço da "próxima" instrução

} InstrucaoFetch;

void fetch(InstrucaoFetch *proximaInstrucao , uint16_t memoria[], uint16_t programaCounter , int houveFlush , uint16_t novoProgramaCounter);

#endif