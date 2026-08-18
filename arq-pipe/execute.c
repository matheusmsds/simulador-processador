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
            case PIPE_ADD:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] + bancoReg[pontDecode->operando2]; //Executando as operações
                pontExecute->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontExecute->regAlvo = pontDecode->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
                break; 

            case PIPE_SUB:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] - bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_MUL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] * bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_DIV:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] / bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_EQUAL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] == bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_NEQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] != bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_LESS:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] < bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_GREATER:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] > bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_LESS_EQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] <= bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_CMP_GREATER_EQ:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] >= bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_AND:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] & bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_OR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] | bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_XOR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] ^ bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_SHL:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] << bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_SHR:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] >> bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_LOAD:
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->acessarMemoria = 1; // leitura
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case PIPE_STORE:
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->dadoParaMemoria = bancoReg[pontDecode->operando2];
                pontExecute->acessarMemoria = 2; // escrita
                break;

            case PIPE_SYSCALL:
                pontExecute->encerrarPrograma = 1; // alteramos o valor da struct via ponteiro, que depois é passado a struct para store() com seu parametro *rodando e é alterado
                break;
        }
    } else {
        switch(pontDecode->opcode) {
            case PIPE_JUMP_COND: {
                uint16_t enderecoReal = 0;
                if (bancoReg[pontDecode->regSalto] != 0) {
                    enderecoReal = pontDecode->imediato;
                }
                int indice = pontDecode->pc;                            // indice / endereço do histórico
                Preditor *pontPred = &historicoPredicoes[indice];       
                pontPred->enderecoAlvo = pontDecode->imediato;          // atualizando o vetor de struct
                pontPred->validade = 1;

                if (enderecoReal != 0) {                                // caso realmente é uma predição válida e deve ocorrer
                    if (pontPred->historicoDesvio < 2) pontPred->historicoDesvio++;    // ++
                } else {
                    if (pontPred->historicoDesvio > -1) pontPred->historicoDesvio--;   // --
                }

                if (pontDecode->enderecoPrevisto == enderecoReal) {     // mesma lógica do JUMP, aqui é comparado se foi acertado ou não, caso sim retorna 0
                    return 0;
                }
                if (enderecoReal != 0) {                                // caso: previsão errou, tinha que ter pulado, retorna o pulo
                    return enderecoReal;
                }
                return pontDecode->pc + 1;                              // caso: previsão errour achou que precisava mudar, mas precisa voltar para o original
            }

            case PIPE_MOV:
                pontExecute->resultadoOPS = pontDecode->imediato; 
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regSalto;
                break;
        }
    }
    return 0;
}