#include <stdio.h>
#include "instru.h"
#include "decode.h"
#include "execute.h"

// execute(), agora passando para a instruct, não em bancoReg, memoria, nem PC, ou seja tá separadinho.
int execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush) { // função com retorno para indicar se vai haver JUMP_COND ou não (0 ou 1)
    *pontExecute = (InstrucaoExecute){0};
    if(houveFlush || !pontDecode->temInstrucao){                            // flush para descartar as instrucoes e retornar
        pontExecute->temInstrucao = 0;
        return 0;   // retorna 0 para sinalizar que nao vai ter jump_cond ou seja é para seguir normalmente
    }
    pontExecute->temInstrucao = 1;
    if(pontDecode->tipoInstrucao == 0) {
        switch(pontDecode->opcode) {
            case ADD:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] + bancoReg[pontDecode->operando2]; //Executando as operações
                pontExecute->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontExecute->regAlvo = pontDecode->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
                break; 

            case SUB:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] - bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case MUL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] * bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case DIV:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] / bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_EQUAL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] == bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_NEQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] != bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_LESS:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] < bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_GREATER:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] > bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_LESS_EQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] <= bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case CMP_GREATER_EQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] >= bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case AND:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] & bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case OR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] | bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case XOR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] ^ bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case SHL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] << bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case SHR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] >> bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case LOAD:
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->acessarMemoria = 1; // leitura
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case STORE:
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->dadoParaMemoria = bancoReg[pontDecode->operando2];
                pontExecute->acessarMemoria = 2; // escrita
                break;

            case SYSCALL:
                pontExecute->encerrarPrograma = 1; // alteramos o valor da struct via ponteiro, que depois é passado a struct para store() com seu parametro *rodando e é alterado
                break;
        }
    } else {
        switch(pontDecode->opcode) {
            case JUMP_COND: {
                uint16_t enderecoReal = 0;
                if (bancoReg[pontDecode->regSalto] != 0) {
                    enderecoReal = pontDecode->imediato;
                }

                int indice = pontDecode->pc % 64;
                Preditor *pontPred = &historicoPredicoes[indice];
                pontPred->enderecoDesvio = pontDecode->pc;
                pontPred->enderecoAlvo = pontDecode->imediato;
                pontPred->validade = 1;

                if (enderecoReal != 0) {
                    if (pontPred->historicoDesvio < 2) pontPred->historicoDesvio++;
                } else {
                    if (pontPred->historicoDesvio > -1) pontPred->historicoDesvio--;
                }

                if (pontDecode->enderecoPrevisto == enderecoReal) {
                    return 0;
                }
                if (enderecoReal != 0) {
                    return enderecoReal;
                }
                return pontDecode->pc + 1;
            }

            case MOV:
                pontExecute->resultadoOPS = pontDecode->imediato; 
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regSalto;
                break;
        }
    }
    return 0;
}