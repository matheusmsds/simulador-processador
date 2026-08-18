#include "fetch.h"
#include "preditor.h"
#include <stdio.h>

void fetch(InstrucaoFetch *pontFetch , uint16_t memoria[] , uint16_t programaCounter , int houveFlush , uint16_t novoProgramaCounter) {
    uint16_t enderecoBusca;                                     // Define qual endereço vai ser tomado, se é novoProgramaCounter ou programaCounter, de acordo se houve ou não um flush das instruções antigas
    if(houveFlush){
        enderecoBusca = novoProgramaCounter;                    // Houve um flush, continuar do novo endereço de memória do novoProgramaCounter
    } else {
        enderecoBusca = programaCounter;                        // Não houve flush, continuar normalmente
    }

    pontFetch->instrucao = memoria[enderecoBusca];              // Pegando a instrucao do endereco
    pontFetch->pc = enderecoBusca;                              // Salvando o lugar da memória que está a instrução
    pontFetch->temInstrucao = 1;                                // Validando se tem a instrução

    if (houveFlush) {
        pontFetch->enderecoPrevisto = 0;                        // Houve um flush, ignoramos o endereçoPrevisto pelo previsor
    } else {
        pontFetch->enderecoPrevisto = previsao(enderecoBusca);  // Não houve flush, então vai continuar fazendo a previsão de endereços
    }
}