#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdint.h>
#include "decode.h"
#include "preditor.h"


typedef struct
{
    uint16_t resultadoOPS;            // Guarda o resultado das operações MOV/LOAD/ADD ...
    uint16_t enderecoMemoria;         // Endereço da memória calculado para LOAD/STORE
    uint16_t dadoParaMemoria;         // Valor a ser escrito na memória (STORE)
    int escreverRegistrador;          // 1 ou 0 indicando se o resultado deve ser escrito em bancoReg[regAlvo]
    int regAlvo;                      // Registrador que receberá o resultado, usado para diferenciar as instruções
    int acessarMemoria;               // 0 = false, 1 = LOAD, 2 = STORE
    int encerrarPrograma;             // 1 se a instrução deve encerrar a execução (syscall serviço 0)
    int temInstrucao;                 // mesma lógica do decode, para saber se tem uma instrucao válida e continuar executando 0 e 1 
    uint16_t enderecoPrevisto;        // mesma lógica do decode, endereco previsto para a próxima instrução

} InstrucaoExecute;

int execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush);
void store(InstrucaoExecute *pontExecute , uint16_t bancoReg[] , uint16_t memoria[] , int *rodando);
void cpuStart(uint16_t memoria[] , uint16_t *programaCounter);

#endif
