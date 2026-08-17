#include "cpuMono.h"
#include "lib.h"

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