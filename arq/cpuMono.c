#include "cpuMono.h"
#include "lib.h"
#include "instru.h"

uint16_t bancoReg[8] = {0};


uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter) {
    return memoria[programaCounter];
}

void decode(uint16_t instrucao , InstrucaoDecodificada *pontMemoria) {
    pontMemoria->tipoInstrucao = extract_bits(instrucao , 15 , 1);

    switch(pontMemoria->tipoInstrucao) {
        case 0: // caso R
            pontMemoria->opcode = extract_bits(instrucao , 9 , 6);
            pontMemoria->regDestino = extract_bits(instrucao , 6 , 3);
            pontMemoria->operando1 = extract_bits(instrucao , 3 , 3);
            pontMemoria->operando2 = extract_bits(instrucao , 0 , 3);
            break;

        case 1: // caso I
            pontMemoria->opcode = extract_bits(instrucao , 13 , 2);
            pontMemoria->regSalto = extract_bits(instrucao , 10 , 3);
            pontMemoria->imediato = extract_bits(instrucao , 0 , 10);
            break;
    }
}

// execute() SÓ CALCULA. Não escreve em bancoReg, memoria, nem PC.
void execute(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[]) {
    if(pontMemoria->tipoInstrucao == 0) {
        switch(pontMemoria->opcode) {
            case ADD:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] + bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case SUB:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] - bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case MUL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] * bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case DIV:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] / bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_EQUAL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] == bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_NEQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] != bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_LESS:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] < bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_GREATER:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] > bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_LESS_EQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] <= bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case CMP_GREATER_EQ:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] >= bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case AND:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] & bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case OR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] | bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case XOR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] ^ bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case SHL:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] << bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case SHR:
                pontMemoria->resultadoOPS = bancoReg[pontMemoria->operando1] >> bancoReg[pontMemoria->operando2];
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case LOAD:
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontMemoria->enderecoMemoria = bancoReg[pontMemoria->operando1];
                pontMemoria->acessarMemoria = 1; // leitura
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regDestino;
                break;

            case STORE:
                pontMemoria->enderecoMemoria = bancoReg[pontMemoria->operando1];
                pontMemoria->dadoParaMemoria = bancoReg[pontMemoria->operando2];
                pontMemoria->acessarMemoria = 2; // escrita
                break;

            case SYSCALL:
                // Serviço 0 (r0 == 0) encerra o programa
                if (bancoReg[0] == 0) {
                    pontMemoria->encerrarPrograma = 1;
                }
                break;
        }
    } else {
        switch(pontMemoria->opcode) {
            case JUMP:
                pontMemoria->novoPC = pontMemoria->imediato;
                pontMemoria->atualizaPC = 1;
                break;

            case JUMP_COND:
                if (bancoReg[pontMemoria->regSalto] != 0) {
                    pontMemoria->novoPC = pontMemoria->imediato;
                    pontMemoria->atualizaPC = 1;
                }
                break;

            case MOV:
                pontMemoria->resultadoOPS = pontMemoria->imediato;
                pontMemoria->escreverRegistrador = 1;
                pontMemoria->regAlvo = pontMemoria->regSalto;
                break;
        }
    }
}

// store() é o estágio de acesso à memória + write-back: aqui sim os efeitos
// colaterais acontecem de verdade (escrever em bancoReg, memoria e PC).
void store(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter , int *rodando) {
    // Acesso à memória (quando a instrução exige)
    if (pontMemoria->acessarMemoria == 1) {          // LOAD: lê da memória
        pontMemoria->resultadoOPS = memoria[pontMemoria->enderecoMemoria];

    } else if (pontMemoria->acessarMemoria == 2) {    // STORE: escreve na memória
        memoria[pontMemoria->enderecoMemoria] = pontMemoria->dadoParaMemoria;
    }

    // Write-back: grava o resultado no banco de registradores
    if (pontMemoria->escreverRegistrador) {
        bancoReg[pontMemoria->regAlvo] = pontMemoria->resultadoOPS;
    }

    // Atualização do PC nos desvios
    if (pontMemoria->atualizaPC) {
        *programaCounter = pontMemoria->novoPC - 1; // -1 pois cpuStart faz PC++ logo depois
    }

    // Encerramento do programa (syscall serviço 0)
    if (pontMemoria->encerrarPrograma) {
        *rodando = 0;
    }
}

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    while(rodando) {
        uint16_t instrucao = fetch(memoria , *programaCounter);

        InstrucaoDecodificada instrucaoDecodificada = {0};

        decode(instrucao , &instrucaoDecodificada);

        execute(&instrucaoDecodificada , bancoReg);

        store(&instrucaoDecodificada , bancoReg , memoria , programaCounter , &rodando);

        (*programaCounter)++;
    }
}