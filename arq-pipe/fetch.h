#ifndef FETCH_H
#define FETCH_H

#include <stdint.h>

typedef struct
{
    uint16_t  programaCounter;
    uint16_t  instrucao;
    int temInstrucao;

} InstrucaoFetch;

uint16_t fetch(uint16_t memoria[], uint16_t programaCounter , InstrucaoFetch *proximaInstrucao , int houveFlush , int novoProgramaCounter);

#endif