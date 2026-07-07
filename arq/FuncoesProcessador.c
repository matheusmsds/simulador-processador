// Ciclos do processador Monociclo
while(true) {
    // Ciclo Fetch - Mexer com o PC 
    uint16_t instrucao = memoria[programaCounter]; 

    // Ciclo Decode
    uint16_t opcode = {0};
    uint16_t operando1 = {0}; 
    uint16_t operando2 = {0};
    uint16_t registradorDestino = {0};
    uint16_t imediato = {0};
    uint16_t registrador = {0};
    const uint16_t tipoInstrucao = extract_bits(instrucao , 15 , 1); 	// Extraindo o 1º bit para saber que tipo de instrução é

    switch(tipoInstrucao) {
        case 0: // caso R
            opcode = extract_bits(instrucao , 9 , 6);
            registradorDestino = extract_bits(instrucao , 6 , 3);
            operando1 = extract_bits(instrucao , 3 , 3);
            operando2 = extract_bits(instrucao , 0 , 3);
            break;

        case 1: // caso I
            opcode = extract_bits(instrucao , 13 , 2);
            registrador = extract_bits(instrucao , 10 , 3);
            imediato = extract_bits(instrucao , 0 , 10);
            break;
    }

    // Ciclo Execute
    if(tipoInstrucao == 0) {
    switch(opcode) {
            case ADD:

            case SUB:

            case MUL:

            case DIV:

            case CMP_EQUAL:

            case CMP_NEQ:

            case CMP_LESS:

            case CMP_GREATER:

            case CMP_LESS_EQ:

            case CMP_GREATER_EQ:

            case AND:

            case OR:

            case XOR:

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
    // Ciclo Store
    programaCounter++;
}