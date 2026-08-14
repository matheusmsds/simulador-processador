#ifndef FETCH_H
#define FETCH_H

#include <stdint.h>

typedef struct
{
    uint16_t  pc;
    uint16_t  instrucao;
    int temInstrucao;

} InstrucaoFetch;

void fetch(InstrucaoFetch *proximaInstrucao , uint16_t memoria[], uint16_t programaCounter , int houveFlush , uint16_t *novoProgramaCounter);

#endif