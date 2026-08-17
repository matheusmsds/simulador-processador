#include <stdio.h>
#include "instru.h"
#include "decode.h"
#include "execute.h"

// execute(), agora passando para a instruct, não em bancoReg, memoria, nem PC, ou seja tá separadinho.
int execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush) { // função com retorno para indicar se vai haver JUMP_COND ou não (0 ou 1)
    *pontExecute = (InstrucaoExecute){0};
    if(houveFlush || !pontDecode->temInstrucao){                            // flush, ou bolha vinda do decode (pipeline enchendo) ---> bolha
        pontExecute->temInstrucao = 0;
        return 0;
    }
    pontExecute->temInstrucao = 1;
    if(pontDecode->tipoInstrucao == 0) {
        switch(pontDecode->opcode) {
            case ADD:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] + bancoReg[pontDecode->operando2]; //Executando as operações
                pontExecute->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontExecute->regAlvo = pontDecode->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
                    printf("DEBUG add: r%d = r%d(%u) + r%d(%u) = %u\n",
        pontDecode->regDestino, pontDecode->operando1, bancoReg[pontDecode->operando1],
        pontDecode->operando2, bancoReg[pontDecode->operando2], pontExecute->resultadoOPS);
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
                // O serviço (definido por r0) é decidido no store(), que é
                // quem tem acesso à memória pra imprimir strings etc.
                pontExecute->encerrarPrograma = 1; // reaproveitado aqui como "é um syscall"
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

                printf("DEBUG jump_cond: pc=%u previsto=%u real=%u historico=%d\n",
                    pontDecode->pc, pontDecode->enderecoPrevisto, enderecoReal, pontPred->historicoDesvio);

                if (pontDecode->enderecoPrevisto == enderecoReal) {
                    printf("DEBUG -> acertou, return 0\n");
                    return 0;
                }
                if (enderecoReal != 0) {
                    printf("DEBUG -> errou, devia desviar, return %u\n", enderecoReal);
                    return enderecoReal;
                }
                printf("DEBUG -> errou, nao devia desviar, return pc+1 = %u\n", pontDecode->pc + 1);
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