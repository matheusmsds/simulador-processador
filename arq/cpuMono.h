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

    // ---- Campos calculados pelo execute() e usados pelo store() (write-back) ----
    uint16_t resultadoALU;            // Resultado de operações aritméticas/lógicas ou valor lido do MOV/LOAD
    uint16_t enderecoMemoria;         // Endereço calculado para LOAD/STORE
    uint16_t dadoParaMemoria;         // Valor a ser escrito na memória (STORE)
    uint16_t novoPC;                  // Novo valor do PC, calculado nos desvios (JUMP/JUMP_COND)

    int escreverRegistrador;          // 1 se o resultado deve ser escrito em bancoReg[regAlvo]
    int regAlvo;                      // Qual registrador receberá o resultado
    int acessarMemoria;               // 0 = nenhum acesso, 1 = leitura (LOAD), 2 = escrita (STORE)
    int atualizaPC;                   // 1 se o PC deve ser alterado (desvio tomado)
    int encerrarPrograma;             // 1 se a instrução deve encerrar a execução (syscall serviço 0)

} InstrucaoDecodificada;

uint16_t fetch(uint16_t memoria[], uint16_t programaCounter);
void decode(uint16_t instrucao, InstrucaoDecodificada *pontMemoria);
void execute(InstrucaoDecodificada *pontMemoria, uint16_t bancoReg[]);
void store(InstrucaoDecodificada *pontMemoria, uint16_t bancoReg[], uint16_t memoria[], uint16_t *programaCounter, int *rodando);
void cpuStart(uint16_t memoria[], uint16_t *programaCounter);


#endif