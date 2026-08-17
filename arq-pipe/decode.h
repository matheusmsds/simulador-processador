#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include "fetch.h"


typedef struct
{
    uint16_t tipoInstrucao;          // Tipo da instrução
    uint16_t opcode;                 // Opcode
    int temInstrucao;

    // Para instruções do tipo R
    uint16_t operando1;              // Operador 1 - "x"
    uint16_t operando2;              // Operador 2 - "y"
    uint16_t regDestino;              // Registrador que será salvo

    // Para instruções do tipo I
    uint16_t imediato;               // Registrador imediato das instruções do tipo I
    uint16_t regSalto;                // Caso jump ignora, se jump_cond será executado ou não

    uint16_t enderecoPrevisto;
    uint16_t pc;

} InstrucaoDecode;

int decode(InstrucaoFetch *pontFetch , InstrucaoDecode *pontDecode ,  int houveFlush);

#endif