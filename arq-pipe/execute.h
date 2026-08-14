#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdint.h>
#include "decode.h"     // precisa da definição de InstrucaoDecode, usada no protótipo abaixo


typedef struct
{
    uint16_t resultadoOPS;            // Guarda o resultado das operações MOV/LOAD/ADD ...
    uint16_t enderecoMemoria;         // Endereço da memória calculado para LOAD/STORE
    uint16_t dadoParaMemoria;         // Valor a ser escrito na memória (STORE)
    uint16_t novoPC;                  // Novo valor do PC, calculado nos desvios (JUMP/JUMP_COND)

    int escreverRegistrador;          // 1 ou 0 indicando se o resultado deve ser escrito em bancoReg[regAlvo]
    int regAlvo;                      // Registrador que receberá o resultado, usado para diferenciar as instruções
    int acessarMemoria;               // 0 = false, 1 = LOAD, 2 = STORE
    int atualizaPC;                   // 1 se o PC deve ser alterado (desvio tomado -> causa flush)
    int ehSyscall;                    // 1 se é um syscall (o serviço, dado por r0, é tratado no store())
    int temInstrucao;                 // 0 = bolha (nenhuma instrução válida neste ciclo)

} InstrucaoExecute;

void execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush , int novoProgramaCounter);

#endif