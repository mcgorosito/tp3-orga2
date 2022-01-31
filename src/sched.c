/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "task.h"

uint8_t currentTaskId = 0;

task taskList[24] = { };

exception debugInfo  = { };

void sched_init() {
	initTask(RICK_C137_ID, RickC137, RICK_C137_CODE);
	initTask(MORTY_C137_ID, MortyC137, MORTY_C137_CODE);
	initTask(RICK_D248_ID, RickD248, RICK_D248_CODE);
	initTask(MORTY_D248_ID, MortyD248, MORTY_D248_CODE);
	// Cronenbergs
	for (int i = 4; i < 24; i++) {
		initTask(i, Cronenberg, CRONENBERG_CODE);
	}
	// Empieza en el último para que al llamar a sched_nextTask por 1ra vez devuelva cero
	currentTaskId = 23;
}

uint16_t sched_nextTask() {
	// Devolver el selector (offset en la GDT) de la siguiente tarea a ejecutar
    do {
        currentTaskId = (currentTaskId + 1) % 24;
    } while (currentTaskId < 24 && taskList[currentTaskId].status == 0);
    return taskList[currentTaskId].gdt_index;
}

/* Funciones modo debug */
void sched_saveDebugInfo(uint32_t exceptionCode, uint32_t cr0, uint32_t cr2, uint32_t cr3, uint32_t cr4,
						 uint16_t ds, uint16_t es, uint16_t fs, uint16_t gs, uint32_t stack5, uint32_t stack4,
						 uint32_t stack3, uint32_t stack2, uint32_t stack1, uint32_t edi, uint32_t esi,
						 uint32_t ebp, uint32_t espNew, uint32_t ebx, uint32_t edx, uint32_t ecx,
						 uint32_t eax, uint16_t errorCode, uint32_t eip, uint32_t cs, uint16_t eflags,
                         uint32_t esp, uint32_t ss) {

	debugInfo.exceptionCode = exceptionCode;
    debugInfo.errorCode = errorCode;
    debugInfo.eflags = eflags;
    debugInfo.eip = eip;
    debugInfo.esp = esp;
    debugInfo.eax = eax;
    debugInfo.ebx = ebx;
    debugInfo.ecx = ecx;
    debugInfo.edx = edx;
    debugInfo.esi = esi;
    debugInfo.edi = edi;
    debugInfo.ebp = ebp;
    debugInfo.cr0 = cr0;
    debugInfo.cr2 = cr2;
    debugInfo.cr3 = cr3;
    debugInfo.cr4 = cr4;
    debugInfo.cs = cs;
    debugInfo.ds = ds;
    debugInfo.es = es;
    debugInfo.fs = fs;
    debugInfo.gs = gs;
    debugInfo.ss = ss;
    debugInfo.stack1 = stack1;    
    debugInfo.stack2 = stack2;
    debugInfo.stack3 = stack3;
    debugInfo.stack4 = stack4;
    debugInfo.stack5 = stack5;    
}

uint8_t sched_exceptionHasErrorCode(uint8_t exceptionNumber) {
    return exceptionNumber == 8 || (exceptionNumber > 9 && exceptionNumber < 15) 
        || exceptionNumber == 17 || exceptionNumber == 21;
}

/* Funciones auxiliares */
void initTask(uint8_t taskId, uint8_t taskType, uint32_t taskCodeAddr) {
	uint8_t x;
	uint8_t y;
	getRandomPosition(taskId, &x, &y);
	uint32_t phy = positionToPhyAddr(x, y); 
	uint32_t cr3 = mmu_initTaskDir(phy, taskCodeAddr, TASK_ATTRS);
	uint8_t gdtIdx = nextFreeGDTIndex;
	tss_init(cr3, taskId);
    taskList[taskId].status = ALIVE;
    taskList[taskId].x = x;
    taskList[taskId].y = y;
    taskList[taskId].gdt_index = gdtIdx;
    taskList[taskId].cr3 = cr3;
	taskList[taskId].type = taskType;
}

uint32_t positionToPhyAddr(uint8_t x, uint8_t y) {
	uint32_t phy = (y * SCREEN_COLS) + x;
	phy = (phy * TASK_CODE_SIZE) + CRONENBERG_WORLD_START;
	return phy;
}
