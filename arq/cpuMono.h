#ifndef CPU_MONO_H
#define CPU_MONO_H

#include <stdint.h>
extern uint16_t bancoReg[8];

typedef struct
{
    uint16_t tipoInstrucao;          // Tipo da instrução
    uint16_t opcode;                 // Opcode

    // Para instruções do tipo R
    uint16_t operando1;              // Operador 1 - "x"
    uint16_t operando2;              // Operador 2 - "y"
    uint16_t regDestino;              // Registrador que será salvo

    // Para instruções do tipo I
    uint16_t imediato;               // Registrador imediato das instruções do tipo I
    uint16_t regSalto;                // Caso jump ignora, se jump_cond será executado ou não

    // Store e Execute
    uint16_t resultadoOPS;            // Guarda o resultado das operações MOV/LOAD/ADD ...
    uint16_t enderecoMemoria;         // Endereço da memória calculado para LOAD/STORE
    uint16_t dadoParaMemoria;         // Valor a ser escrito na memória (STORE)
    uint16_t novoPC;                  // Valor de PC após os desvios;

    int escreverRegistrador;          // 1 se o resultado deve ser escrito em bancoReg[regAlvo]
    int regAlvo;                      // Registrador que receberá o resultado
    int acessarMemoria;               // 0 = false, 1 = LOAD, 2 = STORE
    int atualizaPC;                   // Se o PC foi alterado atualiza (desvio tomado = 1)
    int encerrarPrograma;             // 1 se a instrução deve encerrar a execução (syscall serviço 0)

} InstrucaoDecodificada;

uint16_t fetch(uint16_t memoria[], uint16_t programaCounter);
void decode(uint16_t instrucao, InstrucaoDecodificada *pontMemoria);
void execute(InstrucaoDecodificada *pontMemoria, uint16_t bancoReg[]);
void store(InstrucaoDecodificada *pontMemoria, uint16_t bancoReg[], uint16_t memoria[], uint16_t *programaCounter, int *rodando);
void cpuStart(uint16_t memoria[], uint16_t *programaCounter);


#endif