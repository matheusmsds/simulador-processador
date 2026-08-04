#include "cpuPipeline.h"
#include "lib.h"

uint16_t bancoReg[8] = {0};

uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter) {
    uint16_t instrucao = memoria[programaCounter]; 
    return memoria[programaCounter];
}

uint16_t decode(uint16_t instrucao , InstrucaoDecodificada *pontMemoria) {
    // Ponteiro de memória para acessar a struct. Pode ser o mesmo ponteiros, pois ele aponta para a struct em si, não para as variáveis.
    pontMemoria-> tipoInstrucao = extract_bits(instrucao , 15 , 1); 	// Extraindo o 1º bit para saber que tipo de instrução é
    pontMemoria-> opcode = {0};
    pontMemoria-> operando1 = {0}; 
    pontMemoria-> operando2 = {0};
    pontMemoria-> regDestino = {0};

    pontMemoria-> imediato = {0};
    pontMemoria-> regSalto = {0};

    switch(tipoInstrucao) {
        case 0: // caso R
            opcode = extract_bits(instrucao , 9 , 6);
            regDestino = extract_bits(instrucao , 6 , 3);
            operando1 = extract_bits(instrucao , 3 , 3);
            operando2 = extract_bits(instrucao , 0 , 3);
            break;

        case 1: // caso I
            opcode = extract_bits(instrucao , 13 , 2);
            regSalto = extract_bits(instrucao , 10 , 3);
            imediato = extract_bits(instrucao , 0 , 10);
            break;
    }
    return InstrucaoDecodificada;
}

void execute(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter) {
    // Ciclo Execute
    // ponteiro que aponta para o struct que retorna a instrução decodificada. Pode ser o mesmo ponteiros, pois ele aponta para a struct em si, não para as variáveis.
    pontMemoria->tipoInstrucao;
    pontMemoria->opcode;
    if(tipoInstrucao == 0) {
        switch(opcode) {
                case ADD:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] + bancoReg[pontMemoria->operando2];
                    break;

                case SUB:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] - bancoReg[pontMemoria->operando2];
                    break;

                case MUL:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] * bancoReg[pontMemoria->operando2];
                    break;

                case DIV:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] / bancoReg[pontMemoria->operando2];
                    break;

                case CMP_EQUAL:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] == bancoReg[pontMemoria->2];
                    break;

                case CMP_NEQ:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] != bancoReg[pontMemoria->2];
                    break;

                case CMP_LESS:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] < bancoReg[pontMemoria->2];
                    break;

                case CMP_GREATER:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] > bancoReg[pontMemoria->2];
                    break;

                case CMP_LESS_EQ:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] >= bancoReg[pontMemoria->2];
                    break;

                case CMP_GREATER_EQ:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] >= bancoReg[pontMemoria->2];
                    break;

                case AND:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] && bancoReg[pontMemoria->2];
                    break;

                case OR:
                    bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] || bancoReg[pontMemoria->2];
                    break;

                case XOR:
                    if(bancoReg[pontMemoria->operando1] == bancoReg[pontMemoria->2]){
                        bancoReg[pontMemoria->regDestino] = 0;
                    }
                    else{
                        bancoReg[pontMemoria->regDestino] = 1;
                    }
                    break;

                case SHL:

                case SHR:

                case LOAD:

                case STORE:

                case SYSCALL:
                    
            }
        }

    else {
        switch(opcode) {
            case JUMP:

            case JUMP_COND:

            case MOV:
        }
    }
}

void store() {
    // Ciclo Store


}

void cpuStart(uint16_t memoria[]) {
    while(true) {
        uint16_t instrucao = fetch();

        uint16_t decodificado = decode(instrucao);

        execute(decodificado);

        write();
        programaCounter++;
    }
}