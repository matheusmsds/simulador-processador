#ifndef PREDITOR_H
#define PREDITOR_H

#include <stdint.h>

typedef struct preditor
{
    uint16_t enderecoDesvio;
    int historicoDesvio;
    uint16_t enderecoAlvo;
    int validade;
} Preditor;

extern Preditor historicoPredicoes[64];

int previsao(uint16_t enderecoAtual);

#endif