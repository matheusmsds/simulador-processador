#include "fetch.h"
#include "preditor.h"
#include <stdio.h>

void fetch(InstrucaoFetch *pontFetch , uint16_t memoria[] , uint16_t programaCounter , int houveFlush , uint16_t novoProgramaCounter) {
    // A instrução BUSCADA é sempre a que está de fato no endereço correto
    // (nunca substituída pela predição - a predição só sugere o PRÓXIMO
    // endereço a ser buscado, não troca a instrução buscada agora).
    uint16_t enderecoBusca = houveFlush ? novoProgramaCounter : programaCounter;

    pontFetch->instrucao = memoria[enderecoBusca];
    pontFetch->pc = enderecoBusca;
    pontFetch->temInstrucao = 1;

    if (houveFlush) {
        // Endereço acabou de ser corrigido por um flush: ainda não faz
        // sentido prever nada pra ele neste ciclo.
        pontFetch->enderecoPrevisto = 0;
    } else {
        // Se ESTA instrução (pelo seu próprio endereço) já foi vista como
        // desvio antes, o preditor sugere onde buscar no PRÓXIMO ciclo.
        pontFetch->enderecoPrevisto = previsao(enderecoBusca);
    }

    printf("DEBUG fetch: pc=%u instrucao=%u houveFlush_usado=%d\n", pontFetch->pc, pontFetch->instrucao, houveFlush);
}