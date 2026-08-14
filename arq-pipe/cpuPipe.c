#include <stdio.h>
#include "cpuPipe.h"
#include "lib.h"
#include "instru.h"
#include "decode.h"
#include "execute.h"

uint16_t bancoReg[8] = {0};

// Latches do pipeline. Inicializados com temInstrucao = 0 (bolha),
// o que naturalmente modela o pipeline "enchendo" nos 2 primeiros ciclos.
InstrucaoFetch   fetchando  = {0};
InstrucaoDecode  decodando  = {0};
InstrucaoExecute executando = {0};


// ---------------------------------------------------------------------
// Estágio 1: Busca de instrução (IF)
// ---------------------------------------------------------------------
uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter , InstrucaoFetch *proximaInstrucao , int houveFlush , int novoProgramaCounter) {
    uint16_t enderecoBusca = houveFlush ? novoProgramaCounter : programaCounter;

    proximaInstrucao->programaCounter = enderecoBusca;
    proximaInstrucao->instrucao = memoria[enderecoBusca];
    proximaInstrucao->temInstrucao = 1;

    return proximaInstrucao->instrucao;
}

// ---------------------------------------------------------------------
// Estágio 2: Decodificação de instrução (ID)
// ---------------------------------------------------------------------
void decode(InstrucaoFetch *pontFetch , InstrucaoDecode *pontDecode , int houveFlush) {
    // Se houve flush (desvio resolvido no EX), a instrução que está no
    // latch de fetch é do caminho errado -> vira bolha.
    if (houveFlush || !pontFetch->temInstrucao) {
        pontDecode->temInstrucao = 0;
        return;
    }

    uint16_t instrucao = pontFetch->instrucao;

    pontDecode->temInstrucao = 1;
    pontDecode->tipoInstrucao = extract_bits(instrucao , 15 , 1);

    switch(pontDecode->tipoInstrucao) {
        case 0: // caso R
            pontDecode->opcode = extract_bits(instrucao , 9 , 6);
            pontDecode->regDestino = extract_bits(instrucao , 6 , 3);
            pontDecode->operando1 = extract_bits(instrucao , 3 , 3);
            pontDecode->operando2 = extract_bits(instrucao , 0 , 3);
            break;

        case 1: // caso I
            pontDecode->opcode = extract_bits(instrucao , 13 , 2);
            pontDecode->regSalto = extract_bits(instrucao , 10 , 3);
            pontDecode->imediato = extract_bits(instrucao , 0 , 10);
            break;
    }
}

// ---------------------------------------------------------------------
// Estágio 3: Leitura de operandos + Execução (a parte "EX" do EX/MEM/WB)
// Só calcula. Não escreve em bancoReg/memoria/PC - quem faz isso é o store().
// ---------------------------------------------------------------------
void execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush , int novoProgramaCounter) {
    (void) houveFlush;          // reservado para o preditor de desvio dinâmico (próximo passo)
    (void) novoProgramaCounter; // idem

    // limpa os campos de controle do ciclo anterior
    pontExecute->escreverRegistrador = 0;
    pontExecute->acessarMemoria = 0;
    pontExecute->atualizaPC = 0;
    pontExecute->ehSyscall = 0;
    pontExecute->novoPC = 0;

    if (!pontDecode->temInstrucao) {
        pontExecute->temInstrucao = 0; // bolha: nada a calcular neste ciclo
        return;
    }

    pontExecute->temInstrucao = 1;

    if(pontDecode->tipoInstrucao == 0) {
        switch(pontDecode->opcode) {
            case ADD:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] + bancoReg[pontDecode->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
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
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->acessarMemoria = 1; // leitura, feita de fato no store()
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case STORE:
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->dadoParaMemoria = bancoReg[pontDecode->operando2];
                pontExecute->acessarMemoria = 2; // escrita, feita de fato no store()
                break;

            case SYSCALL:
                // O serviço (definido por r0) só é tratado no store()
                pontExecute->ehSyscall = 1;
                break;
        }
    } else {
        switch(pontDecode->opcode) {
            case JUMP:
                pontExecute->novoPC = pontDecode->imediato;
                pontExecute->atualizaPC = 1;
                break;

            case JUMP_COND:
                if (bancoReg[pontDecode->regSalto] != 0) {
                    pontExecute->novoPC = pontDecode->imediato;
                    pontExecute->atualizaPC = 1;
                }
                break;

            case MOV:
                pontExecute->resultadoOPS = pontDecode->imediato;
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regSalto;
                break;
        }
    }
}

// ---------------------------------------------------------------------
// Estágio 3 (continuação): Memória + Write-back (a parte "MEM/WB")
// Aqui sim os efeitos colaterais acontecem de verdade.
// ---------------------------------------------------------------------
void store(InstrucaoExecute *pontExecute , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter , int *rodando) {
    if (!pontExecute->temInstrucao) {
        return; // bolha: nada a fazer
    }

    // Acesso à memória (quando a instrução exige)
    if (pontExecute->acessarMemoria == 1) {          // LOAD: lê da memória
        pontExecute->resultadoOPS = memoria[pontExecute->enderecoMemoria];
    } else if (pontExecute->acessarMemoria == 2) {    // STORE: escreve na memória
        memoria[pontExecute->enderecoMemoria] = pontExecute->dadoParaMemoria;
    }

    // Write-back: grava o resultado no banco de registradores
    if (pontExecute->escreverRegistrador) {
        bancoReg[pontExecute->regAlvo] = pontExecute->resultadoOPS;
    }

    // Dispatch dos serviços de syscall (definidos pelo valor de r0)
    if (pontExecute->ehSyscall) {
        switch (bancoReg[0]) {
            case 0: // encerra o programa
                *rodando = 0;
                break;

            case 1: { // imprime string: r1 = endereço, terminada em word 0
                uint16_t endereco = bancoReg[1];
                while (memoria[endereco] != 0) {
                    putchar((char) memoria[endereco]);
                    endereco++;
                }
                break;
            }

            case 2: // imprime nova linha
                putchar('\n');
                break;

            case 3: // imprime inteiro: r1 = valor
                printf("%u", bancoReg[1]);
                break;

            default:
                break; // serviço não implementado
        }
    }

    // Atualização do PC "oficial" quando um desvio é resolvido aqui
    // (o cpuStart() usa isso para saber que precisa dar flush no pipeline)
    if (pontExecute->atualizaPC) {
        *programaCounter = pontExecute->novoPC;
    }
}

// ---------------------------------------------------------------------
// Loop principal: um ciclo de clock por iteração.
// A ordem de chamada é da instrução MAIS VELHA (estágio 3) para a MAIS
// NOVA (estágio 1), para simular a atualização simultânea dos latches:
// cada estágio lê o latch escrito no ciclo ANTERIOR antes que ele seja
// sobrescrito, mais tarde nesse mesmo ciclo, pelo estágio anterior a ele.
// ---------------------------------------------------------------------
void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    int houveFlush = 0;
    uint16_t novoProgramaCounter = 0;

    while (rodando) {
        // --- Estágio 3: Execução + Memória + Write-back ---
        // Usa o latch produzido pelo Decode no ciclo anterior (decodando)
        execute(&decodando , &executando , bancoReg , houveFlush , novoProgramaCounter);
        store(&executando , bancoReg , memoria , programaCounter , &rodando);

        // Um desvio foi resolvido agora: as instruções mais novas que já
        // tinham sido buscadas/decodificadas no caminho errado precisam
        // ser descartadas (flush).
        houveFlush = executando.temInstrucao && executando.atualizaPC;
        if (houveFlush) {
            novoProgramaCounter = executando.novoPC;
        }

        // --- Estágio 2: Decodificação ---
        // Usa o latch produzido pelo Fetch no ciclo anterior (fetchando)
        decode(&fetchando , &decodando , houveFlush);

        // --- Estágio 1: Busca ---
        if (houveFlush) {
            *programaCounter = novoProgramaCounter;
        }
        fetch(memoria , *programaCounter , &fetchando , houveFlush , novoProgramaCounter);

        (*programaCounter)++;
    }
}