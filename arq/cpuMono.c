#include "cpuMono.h"
#include "lib.h"

uint16_t bancoReg[8] = {0};


uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter) {
    uint16_t instrucao = memoria[programaCounter]; 
    return memoria[programaCounter];
}

void decode(uint16_t instrucao , InstrucaoDecodificada *pontMemoria) {
    // Ponteiro de memória para acessar a struct. Pode ser o mesmo ponteiros, pois ele aponta para a struct em si, não para as variáveis.
    pontMemoria-> tipoInstrucao = extract_bits(instrucao , 15 , 1); 	// Extraindo o 1º bit para saber que tipo de instrução é

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

void execute(InstrucaoDecodificada *pontMemoria , uint16_t bancoReg[] , uint16_t memoria[] , uint16_t *programaCounter , int *rodando) {
    // Ciclo Execute
    // ponteiro que aponta para o struct que retorna a instrução decodificada. Pode ser o mesmo ponteiros, pois ele aponta para a struct em si, não para as variáveis.
    if(pontMemoria->tipoInstrucao == 0) {
        switch(pontMemoria->opcode) {
            case 0: //ADD:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] + bancoReg[pontMemoria->operando2];
                break;

            case 1: //SUB:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] - bancoReg[pontMemoria->operando2];
                break;

            case 2: //MUL:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] * bancoReg[pontMemoria->operando2];
                break;

            case 3: //DIV:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] / bancoReg[pontMemoria->operando2];
                break;

            case 4: //CMP_EQUAL:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] == bancoReg[pontMemoria->operando2];
                break;

            case 5: //CMP_NEQ:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] != bancoReg[pontMemoria->operando2];
                break;

            case 6: //CMP_LESS:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] < bancoReg[pontMemoria->operando2];
                break;

            case 7: //CMP_GREATER:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] > bancoReg[pontMemoria->operando2];
                break;

            case 8: //CMP_LESS_EQ:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] <= bancoReg[pontMemoria->operando2];
                break;

            case 9: //CMP_GREATER_EQ:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] >= bancoReg[pontMemoria->operando2];
                break;

            case 10: //AND:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] & bancoReg[pontMemoria->operando2];
                break;

            case 11: //OR:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] | bancoReg[pontMemoria->operando2];
                break;

            case 12: //XOR:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] ^ bancoReg[pontMemoria->operando2];
                break;

            case 13: //SHL:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] << bancoReg[pontMemoria->operando2];
                break;

            case 14: //SHR:
                bancoReg[pontMemoria->regDestino] = bancoReg[pontMemoria->operando1] >> bancoReg[pontMemoria->operando2];
                break;
                
            case 15: //LOAD:
                bancoReg[pontMemoria->regDestino] = memoria[bancoReg[pontMemoria->operando1]];
                break;

            case 16: //STORE:
                memoria[bancoReg[pontMemoria->operando1]] = bancoReg[pontMemoria->operando2];
                break;

            case 63: //SYSCALL:
                *rodando = 0;
        }
    } else {
        switch(pontMemoria->opcode) {
            case 0: //JUMP:
                *programaCounter = pontMemoria->imediato;
                *programaCounter -= 1;
                break;

            case 1: //JUMP_COND:
                if(bancoReg[pontMemoria->regSalto] != 0){
                    *programaCounter = pontMemoria->imediato;
                    *programaCounter -= 1;
                }
                break;

            case 3: //MOV:
                bancoReg[pontMemoria->regSalto] = pontMemoria->imediato;
                break;
        }
    }
}

void store() {
    // Ciclo Store
}

void cpuStart(uint16_t memoria[] , uint16_t *programaCounter) {
    int rodando = 1;
    while(rodando) {
        uint16_t instrucao = fetch(memoria , *programaCounter);

        InstrucaoDecodificada instrucaoDecodificada = {0};

        decode(instrucao , &instrucaoDecodificada);

        execute(&instrucaoDecodificada , bancoReg , memoria , programaCounter , &rodando);

        store();

        (*programaCounter)++;
    }
}