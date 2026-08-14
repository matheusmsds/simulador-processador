#include "cpuMono.h"
#include "lib.h"
#include "instru.h"

uint16_t bancoReg[8] = {0};

InstrucaoDecodificada instrucaoDecodificada = {0};
InstrucaoFetch fetchando = {0};
InstrucaoDecode decodando = {0};
InstrucaoExecute executando = {0};


uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter) {
    return memoria[programaCounter];
}

void decode(uint16_t instrucao , InstrucaoDecode *pontDecode) {
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

// execute(), agora passando para a instruct, não em bancoReg, memoria, nem PC, ou seja tá separadinho.
void execute(InstrucaoDecode *pontExecute , uint16_t bancoReg[]) {
    if(pontExecute->tipoInstrucao == 0) {
        switch(pontExecute->opcode) {
            case ADD:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] + bancoReg[pontExecute->operando2]; //Executando as operações
                pontExecute->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontExecute->regAlvo = pontExecute->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
                break; 

            case SUB:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] - bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case MUL:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] * bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case DIV:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] / bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_EQUAL:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] == bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_NEQ:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] != bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_LESS:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] < bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_GREATER:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] > bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_LESS_EQ:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] <= bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case CMP_GREATER_EQ:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] >= bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case AND:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] & bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case OR:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] | bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case XOR:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] ^ bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case SHL:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] << bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case SHR:
                pontExecute->resultadoOPS = bancoReg[pontExecute->operando1] >> bancoReg[pontExecute->operando2];
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case LOAD:
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontExecute->enderecoMemoria = bancoReg[pontExecute->operando1];
                pontExecute->acessarMemoria = 1; // leitura
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regDestino;
                break;

            case STORE:
                pontExecute->enderecoMemoria = bancoReg[pontExecute->operando1];
                pontExecute->dadoParaMemoria = bancoReg[pontExecute->operando2];
                pontExecute->acessarMemoria = 2; // escrita
                break;

            case SYSCALL:
                // Serviço 0 (r0 == 0) encerra o programa
                if (bancoReg[0] == 0) {
                    pontExecute->encerrarPrograma = 1;  // Muda o valor de encerrarPrograma por ponteiro
                }
                break;
        }
    } else {
        switch(pontExecute->opcode) {
            case JUMP:
                pontExecute->novoPC = pontExecute->imediato; // novoPC recebe o valor imediato da instrução
                pontExecute->atualizaPC = 1; // mesma lógica do escreverRegistrador, indica se foi desviado ou não
                break;

            case JUMP_COND:
                if (bancoReg[pontExecute->regSalto] != 0) {
                    pontExecute->novoPC = pontExecute->imediato;
                    pontExecute->atualizaPC = 1;
                }
                break;

            case MOV:
                pontExecute->resultadoOPS = pontExecute->imediato; 
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontExecute->regSalto;
                break;
        }
    }
}

//Agora sim é para salvar as instruções
void store(InstrucaoExecute *pontExecute , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter , int *rodando) {
    // Caso precise, acessa a memória 
    if (pontExecute->acessarMemoria == 1) {          // LOAD lê da memória
        pontExecute->resultadoOPS = memoria[pontExecute->enderecoMemoria];

    } else if (pontExecute->acessarMemoria == 2) {    // STORE escreve na memória
        memoria[pontExecute->enderecoMemoria] = pontExecute->dadoParaMemoria;
    }

    // Write-back: grava o resultado no banco de registradores
    if (pontExecute->escreverRegistrador) {
        bancoReg[pontExecute->regAlvo] = pontExecute->resultadoOPS;
    }

    // Atualização do PC nos desvios
    if (pontExecute->atualizaPC) {
        *programaCounter = pontExecute->novoPC - 1; // -1 pois cpuStart faz PC++ logo depois
    }

    // Encerramento do programa (syscall serviço 0)
    if (pontExecute->encerrarPrograma) {
        *rodando = 0;
    }
}

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    int houveFlush = 0;
    uint16_t novoProgramaCounter = 0;
    while(rodando) {
        
        uint16_t instrucao = fetch(memoria , *programaCounter);

        decode(instrucao , &instrucaoDecode , *houveFlush , *novoProgramaCounter);

        execute(&InstrucaoExecute , bancoReg , *houveFlush , *novoProgramaCounter);

        store(&InstrucaoExecute , bancoReg , memoria , programaCounter , &rodando);

        (*programaCounter)++;
    }
}