#include "lib.h"
#include <stdio.h>
#include "fetch.h"
#include "decode.h"
#include "preditor.h"
#include "instru.h"

int decode(InstrucaoFetch *pontFetch , InstrucaoDecode *pontDecode , int houveFlush) {  // função com retorno para indicar se vai haver JUMP ou não (0 ou 1)
    uint16_t instrucao = pontFetch->instrucao;
    printf("DEBUG decode: pc=%u houveFlush=%d\n", pontFetch->pc, houveFlush);
    if(houveFlush){                                                         // indica se teve flush, caso tenha ---> bolha, caso não ---> decode normalmente
        pontDecode->temInstrucao = 0;
        return 0;
    } 
    else {
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

        pontDecode->enderecoPrevisto = pontFetch->enderecoPrevisto;
        pontDecode->pc = pontFetch->pc;

        if(pontDecode->tipoInstrucao && pontDecode->opcode == JUMP) {       // tratando o JUMP dentro de decode
            int indice = pontFetch-> pc % 64;
            Preditor *pontPred = &historicoPredicoes[indice];
            pontPred->enderecoDesvio = pontFetch->pc;
            pontPred->enderecoAlvo = pontDecode->imediato;
            pontPred->validade = 1;
            if(pontPred->historicoDesvio < 2){
                pontPred->historicoDesvio++;
            }
            if(pontDecode->enderecoPrevisto == pontDecode->imediato){
                return 0;
            }
            return pontDecode->imediato;
        }
    }
    return 0;
}