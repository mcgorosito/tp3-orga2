/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "tss.h"

typedef enum e_taskType {
    RickC137       = 1,
    MortyC137      = 2,
    RickD248       = 3,
    MortyD248      = 4,
    Cronenberg     = 5,
    CronenbergC137 = 6,
    CronenbergD248 = 7,
} e_taskType_t;

typedef struct taskInfo {  // ID es la posición en el array
    e_taskType_t type;     // Task type
    uint8_t status;        // 1 = alive, 0 = dead
    uint8_t x;             // position
    uint8_t y;
    uint8_t gdt_index;
    uint32_t cr3;
} __attribute__((__packed__, aligned (8))) task;

typedef struct exceptionInfo {
    uint32_t exceptionCode;
    uint32_t errorCode;
    uint32_t eflags;
    uint32_t eip;
    uint32_t esp;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t cr0;
    uint32_t cr2;
    uint32_t cr3;
    uint32_t cr4;
    uint16_t cs;
    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint32_t stack1;
    uint32_t stack2;
    uint32_t stack3;
    uint32_t stack4;
    uint32_t stack5;
} __attribute__((__packed__, aligned (8))) exception;

/* Variables globales*/
// Tarea que se está ejecutando actualmente
extern uint8_t currentTaskId;  // posición de la tarea en taskList
// Lista de tareas con toda la información necesaria
extern task taskList[24];
// Información sobre la última excepción para modo debug
extern exception debugInfo;

void sched_init();
uint16_t sched_nextTask();

/* Funciones modo debug */
void sched_saveDebugInfo(uint32_t exceptionCode, uint32_t cr0, uint32_t cr2, uint32_t cr3, uint32_t cr4,
                         uint16_t ds, uint16_t es, uint16_t fs, uint16_t gs, uint32_t stack5, uint32_t stack4,
                         uint32_t stack3, uint32_t stack2, uint32_t stack1, uint32_t edi, uint32_t esi,
                         uint32_t ebp, uint32_t espNew, uint32_t ebx, uint32_t edx, uint32_t ecx,
                         uint32_t eax, uint16_t errorCode, uint32_t eip, uint32_t cs, uint16_t eflags,
                         uint32_t esp, uint32_t ss);
uint8_t sched_exceptionHasErrorCode(uint8_t exceptionNumber);

/* Funciones auxiliares */
void initTask(uint8_t taskId, uint8_t taskType, uint32_t taskCodeAddr);
uint32_t positionToPhyAddr(uint8_t x, uint8_t y);

#endif	/* !__SCHED_H__ */
