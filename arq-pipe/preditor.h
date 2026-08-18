#ifndef PREDITOR_H
#define PREDITOR_H

#include <stdint.h>

typedef struct preditor
{
    uint16_t enderecoDesvio;                // endereço de desvio
    int historicoDesvio;                    // historico do desvio (-1, 0, 1, 2) se acertou ou não
    uint16_t enderecoAlvo;                  // endereço alvo para desviar na memoria
    int validade;                           // indica se é valido ou não (0 ou 1)
} Preditor;

extern Preditor historicoPredicoes[64];     // vetor que armazena struct preditor, basicamente: está armazenando as predições

int previsao(uint16_t enderecoAtual);       //

#endif