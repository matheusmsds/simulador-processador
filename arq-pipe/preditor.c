#include "stdio.h"
#include "preditor.h"

Preditor historicoPredicoes[65536];                                                 // histórico das predições

int previsao(uint16_t enderecoAtual){                                               // basicamente a função vai consultar o histórico das predições
    int indice = enderecoAtual;                                                     // pega o indice ("endereço") da instrução e vai criar um lugar para ela
    Preditor *pontPredit = &historicoPredicoes[indice];                             // utilizando ponteiro para acessar os valores dentro do vetor e da posição
    if(pontPredit->validade){                                                       // se for valido o salto por exemplo
        if(pontPredit->historicoDesvio == -1 || pontPredit->historicoDesvio == 0){  // se é pouco provável d ser um desvio retorna 0 para false para o programa continuar
            return 0;
        }
        else{                                                                       // se não, vai retornar o valor do enderecoAlvo (indice)
            return pontPredit->enderecoAlvo;
        }
    }
    return 0;
}