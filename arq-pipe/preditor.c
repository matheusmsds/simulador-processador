#include "stdio.h"
#include "preditor.h"

Preditor historicoPredicoes[64];

int previsao(uint16_t enderecoAtual){
    int indice = enderecoAtual % 64;
    Preditor *pontPredit = &historicoPredicoes[indice];
    if(pontPredit->enderecoDesvio == enderecoAtual && pontPredit->validade){
        if(pontPredit->historicoDesvio == -1 || pontPredit->historicoDesvio == 0){
            return 0;
        }
        else{
            return pontPredit->enderecoAlvo;
        }
    }
    return 0;
}