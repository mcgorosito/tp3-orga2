/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

const uint8_t valuesX[24] = {5, 23, 13, 35, 30, 79, 21, 10, 73, 32, 70, 28, 73, 64, 28, 12,  3, 41, 27, 24, 54, 20, 33, 74};
const uint8_t valuesY[24] = {14, 2, 39, 25, 37, 29, 12, 24,  0,  4,  2, 16, 15, 24, 24, 18, 23, 38, 12, 25, 14, 22,  0, 28};

void getRandomPosition(uint8_t index, uint8_t *x, uint8_t *y) {
    *x = valuesX[index%24];
    *y = valuesY[index%24];
}
