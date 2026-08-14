#ifndef CPU_PIPE_H
#define CPU_PIPE_H

#include <stdint.h>
#include "fetch.h"
#include "decode.h"
#include "execute.h"

extern uint16_t bancoReg[8];

// Registradores de pipeline (latches) entre os estágios.
// Persistem entre ciclos de clock - são o "estado" do pipeline.
extern InstrucaoFetch   fetchando;    // latch IF  -> ID
extern InstrucaoDecode  decodando;    // latch ID  -> EX/MEM/WB
extern InstrucaoExecute executando;   // resultado calculado no estágio EX/MEM/WB

// fetch(), decode() e execute() já são declaradas em fetch.h / decode.h / execute.h

void store(InstrucaoExecute *pontExecute, uint16_t bancoReg[], uint16_t memoria[], uint16_t *programaCounter, int *rodando);
void cpuStart(uint16_t memoria[], uint16_t *programaCounter);

#endif