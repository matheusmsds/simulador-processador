#include "lib.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "instru.h"

uint16_t bancoReg[8] = {0};
uint16_t novoProgramaCounter = 0;

InstrucaoFetch fetchando = {0};
InstrucaoDecode decodando = {0};
InstrucaoExecute executando = {0};

void fetch(InstrucaoFetch *pontFetch , uint16_t memoria[] , uint16_t programaCounter , int houveFlush , uint16_t novoProgramaCounter) {
    if(houveFlush){                                                         // indica se teve flush, caso tenha ---> bolha, caso não ---> decode normalmente
        pontFetch->instrucao = memoria[novoProgramaCounter];
        pontFetch->pc = novoProgramaCounter;

    }
    else {
        pontFetch->instrucao = memoria[programaCounter];
        pontFetch->pc = programaCounter;
    }
    pontFetch->temInstrucao = 1;

}

int decode(InstrucaoFetch *pontFetch , InstrucaoDecode *pontDecode , int houveFlush) {  // função com retorno para indicar se vai haver JUMP ou não (0 ou 1)
    uint16_t instrucao = pontFetch->instrucao;
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
        if(pontDecode->tipoInstrucao && pontDecode->opcode == JUMP) {       // tratando o JUMP dentro de decode
            return pontDecode->imediato;
        }
    }
    return 0;
}

// execute(), agora passando para a instruct, não em bancoReg, memoria, nem PC, ou seja tá separadinho.
int execute(InstrucaoDecode *pontDecode , InstrucaoExecute *pontExecute , uint16_t bancoReg[] , int houveFlush) { // função com retorno para indicar se vai haver JUMP_COND ou não (0 ou 1)
    *pontExecute = (InstrucaoExecute){0};
    if(houveFlush){                                                         // // indica se teve flush, caso tenha ---> bolha, caso não ---> decode normalmente
        pontExecute->temInstrucao = 0;
        return 0;
    }
    if(pontDecode->tipoInstrucao == 0) {
        switch(pontDecode->opcode) {
            case ADD:
                pontExecute->resultadoOPS = bancoReg[pontDecode->operando1] + bancoReg[pontDecode->operando2]; //Executando as operações
                pontExecute->escreverRegistrador = 1; // indica se deve ser escrito ou não no store
                pontExecute->regAlvo = pontDecode->regDestino; // Guarda aonde será salvo no banco dos registradores, além de mascarar da onde vem
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
                // Calcula o endereço agora; a leitura de fato acontece no store() (acesso à memória)
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->acessarMemoria = 1; // leitura
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regDestino;
                break;

            case STORE:
                pontExecute->enderecoMemoria = bancoReg[pontDecode->operando1];
                pontExecute->dadoParaMemoria = bancoReg[pontDecode->operando2];
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
        switch(pontDecode->opcode) {
            case JUMP_COND:
                if (bancoReg[pontDecode->regSalto] != 0) {
                    //pontExecute->escreverRegistrador = 0;   // APENAS DESVIARÁ SEM ESCREVER;
                    //pontExecute->novoPC = pontExecute->imediato; // novoPC recebe o valor imediato da instrução
                    //pontExecute->atualizaPC = 1;    // mesma lógica do escreverRegistrador, indica se foi desviado ou não
                    return pontDecode->imediato;
                }
                break;

            case MOV:
                pontExecute->resultadoOPS = pontDecode->imediato; 
                pontExecute->escreverRegistrador = 1;
                pontExecute->regAlvo = pontDecode->regSalto;
                break;
        }
    }
    return 0;
}

//Agora sim é para salvar as instruções
void store(InstrucaoExecute *pontExecute , uint16_t bancoReg[] , uint16_t memoria[] , int *rodando) {
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

    // Encerramento do programa (syscall serviço 0)
    if (pontExecute->encerrarPrograma) {
        *rodando = 0;
    }
}

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    int houveFlush = 0;

    while(rodando) {

        store(&executando , bancoReg , memoria , &rodando);

        int executar = execute(&decodando , &executando , bancoReg , houveFlush);

        int decodar = decode(&fetchando , &decodando , houveFlush);

        uint16_t novoProgramaCounter = 0;
        if(executar != 0){ // vai ter JUMP_COND
            novoProgramaCounter = executar;
        }

        else if (decodar != 0){ // VAI TER JUMP
             novoProgramaCounter = decodar;
        }
        if (novoProgramaCounter != 0) {
            *programaCounter = novoProgramaCounter;
        }

        houveFlush = (novoProgramaCounter != 0);


        fetch(&fetchando , memoria , *programaCounter , houveFlush , novoProgramaCounter);

        if(!houveFlush){
            (*programaCounter)++;
        }
    }
}
