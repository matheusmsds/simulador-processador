#include "cpuMono.h"
#include "lib.h"
#include "instru.h"

// execute(), agora passando para a instruct, não em bancoReg, memoria, nem PC, ou seja tá separadinho.
void execute(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[]) {
    if(pontMemoria->tipoInstrucao == 0) {
        switch(pontMemoria->opcode) {
            case MONO_ADD:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] + bancoReg[pontMemoria->operando2]; //Executando as operações
                pontMemoria->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontMemoria->regAlvo = pontMemoria->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
                break; 

            case MONO_SUB:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] - bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_MUL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] * bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_DIV:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] / bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_EQUAL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] == bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_NEQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] != bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_LESS:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] < bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_GREATER:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] > bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_LESS_EQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] <= bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_CMP_GREATER_EQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] >= bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_AND:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] & bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_OR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] | bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_XOR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] ^ bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_SHL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] << bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_SHR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] >> bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_LOAD:
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontMemoria->enderecoMemoria = bancoReg[pontMemoria->operando1];
                pontMemoria->acessarMemoria = 1; // leitura
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MONO_STORE:
                pontMemoria->enderecoMemoria = bancoReg[pontMemoria->operando1];
                pontMemoria->dadoParaMemoria = bancoReg[pontMemoria->operando2];
                pontMemoria->acessarMemoria = 2; // escrita
                break;

            case MONO_SYSCALL:
                // Serviço 0 (r0 == 0) encerra o programa
                if (bancoReg[0] == 0) {
                    pontMemoria->encerrarPrograma = 1;  // Muda o valor de encerrarPrograma por ponteiro
                }
                break;
        }
    } else {
        switch(pontMemoria->opcode) {
            case MONO_JUMP:
                pontMemoria->novoPC = pontMemoria->imediato; // novoPC recebe o valor imediato da instrução
                pontMemoria->atualizaPC = 1; // mesma lógica do escreverRegistrador, indica se foi desviado ou não
                break;

            case MONO_JUMP_COND:
                if (bancoReg[pontMemoria->regSalto] != 0) {
                    pontMemoria->novoPC = pontMemoria->imediato;
                    pontMemoria->atualizaPC = 1;
                }
                break;

            case MONO_MOV:
                pontMemoria->resultadoOPS = pontMemoria->imediato; 
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regSalto;
                break;
        }
    }
}