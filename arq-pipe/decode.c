#include "lib.h"
#include <stdio.h>
#include "fetch.h"
#include "decode.h"
#include "preditor.h"
#include "instru.h"

int decode(InstrucaoFetch *pontFetch , InstrucaoDecode *pontDecode , int houveFlush) {  // função com retorno para indicar se vai haver JUMP ou não (0 ou 1)
    uint16_t instrucao = pontFetch->instrucao;
    if(houveFlush || !pontFetch->temInstrucao){                             // flush / limpeza do processador, (fetch anterior não tinha instrução real)
        pontDecode->temInstrucao = 0;                                       // instrução inválida
        return 0;
    } 
    else {
        pontDecode->temInstrucao = 1;                                       // instrucao válida
        pontDecode->tipoInstrucao = extract_bits(instrucao , 15 , 1);       // pegando o primeiro bit, da esquerda para direita

        switch(pontDecode->tipoInstrucao) {
            case 0: // caso R
                pontDecode->opcode = extract_bits(instrucao , 9 , 6);       // pegando os 9 bits depois do primeiro
                pontDecode->regDestino = extract_bits(instrucao , 6 , 3);   // pegando os 3 bits depois dos bits de opcode
                pontDecode->operando1 = extract_bits(instrucao , 3 , 3);    // pegando os 3 bits depois do registrador de destino
                pontDecode->operando2 = extract_bits(instrucao , 0 , 3);    // pegando os 3 bits depois do operando 1
                break;

            case 1: // caso I
                pontDecode->opcode = extract_bits(instrucao , 13 , 2);      // pegando os 2 bits depois do primeiro
                pontDecode->regSalto = extract_bits(instrucao , 10 , 3);    // pegando os 3 bits depois do opcode
                pontDecode->imediato = extract_bits(instrucao , 0 , 10);    // pegando os 10 bits depois do registrador de Salto
                break;
        }

        // passando as instruções de um estágio para outro (Decode recebendo o endereço previsto e o programaConter (pc))
        pontDecode->enderecoPrevisto = pontFetch->enderecoPrevisto;
        pontDecode->pc = pontFetch->pc;

        if(pontDecode->tipoInstrucao && pontDecode->opcode == PIPE_JUMP) {       // tratando o JUMP dentro de decode
            int indice = pontFetch-> pc;                                    // transformando em indice para armazenamento da predicao e para não termos repetições de indices.
            Preditor *pontPred = &historicoPredicoes[indice];               // armazenando o enderecoDesvio, o enderecoAlvo, se é válido e se acertou
            pontPred->enderecoAlvo = pontDecode->imediato;                  // atualiza o endereço 
            pontPred->validade = 1;                                         // marca validade do jump
            if(pontPred->historicoDesvio < 2){
                pontPred->historicoDesvio++;                                // atualizando o "histórico de ocorrência"
            }
            if(pontDecode->enderecoPrevisto == pontDecode->imediato){       // aqui é comparado se foi acertado ou não, caso sim retorna 0
                return 0;
            }
            return pontDecode->imediato;                                    // caso contrário retorna o imediato para realizar o pulo
        }
    }
    return 0;
}