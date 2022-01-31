/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "stdint.h"
#include "colors.h"
#include "defines.h"
#include "sched.h"
#include "game.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    uint8_t c;
    uint8_t a;
} ca;

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);

void screen_printKeyboardNumber(uint8_t scancode);
void screen_drawBox(uint32_t fInit, uint32_t cInit,
                    uint32_t fSize, uint32_t cSize,
                    uint8_t character, uint8_t attr );
void screen_printNumeroLibreta();
void screen_printException(uint32_t code);
void screen_drawTasks();
void screen_printEndOfGame();
void screen_clearBoard();
void screen_clearClocks();
void screen_printCronenbergTable();
void screen_printPlayerScores(uint8_t score_red, uint8_t score_blue);
void screen_printDebugModeOn();
void screen_clearDebugMode();
void screen_printDebugBox();

/* Funciones auxiliares */
char* getTaskLetter(e_taskType_t taskType);
uint16_t getTaskColor(e_taskType_t taskType);

#endif  /* !__SCREEN_H__ */