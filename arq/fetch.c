#include "cpuMono.h"
#include "lib.h"

uint16_t fetch(uint16_t memoria[] , uint16_t programaCounter) {
    return memoria[programaCounter];
}