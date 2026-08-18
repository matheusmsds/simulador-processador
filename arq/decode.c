#include "cpuMono.h"
#include "lib.h"

void decode(uint16_t instrucao , InstrucaoDecodificada *pontMemoria) {
    pontMemoria->tipoInstrucao = extract_bits(instrucao , 15 , 1);          // pegando o primeiro bit, da esquerda para direita

    switch(pontMemoria->tipoInstrucao) {
        case 0: // caso R
            pontMemoria->opcode = extract_bits(instrucao , 9 , 6);          // pegando os 9 bits depois do primeiro
            pontMemoria->regDestino = extract_bits(instrucao , 6 , 3);      // pegando os 3 bits depois dos bits de opcode
            pontMemoria->operando1 = extract_bits(instrucao , 3 , 3);       // pegando os 3 bits depois do registrador de destino
            pontMemoria->operando2 = extract_bits(instrucao , 0 , 3);       // pegando os 3 bits depois do operando 1
            break;

        case 1: // caso I
            pontMemoria->opcode = extract_bits(instrucao , 13 , 2);         // pegando os 2 bits depois do primeiro
            pontMemoria->regSalto = extract_bits(instrucao , 10 , 3);       // pegando os 3 bits depois do opcode
            pontMemoria->imediato = extract_bits(instrucao , 0 , 10);       // pegando os 10 bits depois do registrador de Salto
            break;
    }
}