/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;
tss tss_tasks[24];

uint8_t nextFreeGDTIndex = 15;

void tss_init_initial() {
	// No hay que hacer nada porque va todo en cero
	// Agregar la base en la entrada de la GDT
    gdt[GDT_IDX_TSS_INITIAL].base_0_15 = (uint32_t)&tss_initial;
    gdt[GDT_IDX_TSS_INITIAL].base_23_16 = (uint32_t)(&tss_initial) >> 16;
    gdt[GDT_IDX_TSS_INITIAL].base_31_24 = (uint32_t)(&tss_initial) >> 24;
}

void tss_init_idle() {
	tss_idle.eip = IDLE;             // dir de inicio del código
	tss_idle.cr3 = KERNEL_PAGE_DIR;  // mismo cr3 que el kernel
	
	tss_idle.eflags = EFLAGS_WITH_INT;
	
    tss_idle.esp = KERNEL_STACK_BASE;
	tss_idle.esp0 = KERNEL_STACK_BASE;
	tss_idle.ss0 = GDT_OFF_DS_RING_0 | RPL_LVL_0;
    
    // Selectores de segmento
	tss_idle.cs = GDT_OFF_CS_RING_0 | RPL_LVL_0;
    tss_idle.ss = GDT_OFF_DS_RING_0 | RPL_LVL_0;
    tss_idle.ds = GDT_OFF_DS_RING_0 | RPL_LVL_0;
    tss_idle.es = GDT_OFF_DS_RING_0 | RPL_LVL_0;         
    tss_idle.fs = GDT_OFF_DS_RING_0 | RPL_LVL_0;
    tss_idle.gs = GDT_OFF_DS_RING_0 | RPL_LVL_0;

    tss_idle.iomap = IO_MAP_DISABLED;    // Puertos deshabilitados

	// Agregar la base en la entrada de la GDT	
    gdt[GDT_IDX_TSS_IDLE].base_0_15 = (uint32_t)&tss_idle;
    gdt[GDT_IDX_TSS_IDLE].base_23_16 = (uint32_t)(&tss_idle) >> 16;
    gdt[GDT_IDX_TSS_IDLE].base_31_24 = (uint32_t)(&tss_idle) >> 24;
}

/* Toma como parametro el cr3 del mapa de memoria creado para la tarea */
void tss_init(uint32_t task_cr3, uint8_t id) {
	// 	Pedir una página del área libre de kernel para la pila de nivel 0
	uint32_t stack_lvl_0_addr = mmu_nextFreeKernelPage();

	// Crear un nuevo struct tss con los datos correspondientes a la tarea
	tss_tasks[id].cr3 = task_cr3;
	tss_tasks[id].eip = TASK_CODE_INIT;
	tss_tasks[id].esp = TASK_CODE_INIT + 2*PAGE_SIZE;
	tss_tasks[id].eflags = EFLAGS_WITH_INT;

	// completar selectores de segmento CS, DS, ..., SS
    tss_tasks[id].cs = GDT_OFF_CS_RING_3 | RPL_LVL_3;
    tss_tasks[id].ss = GDT_OFF_DS_RING_3 | RPL_LVL_3;
    tss_tasks[id].ds = GDT_OFF_DS_RING_3 | RPL_LVL_3;
    tss_tasks[id].es = GDT_OFF_DS_RING_3 | RPL_LVL_3;         
    tss_tasks[id].fs = GDT_OFF_DS_RING_3 | RPL_LVL_3;
    tss_tasks[id].gs = GDT_OFF_DS_RING_3 | RPL_LVL_3;

    tss_tasks[id].iomap = IO_MAP_DISABLED;

   	tss_tasks[id].esp0 = stack_lvl_0_addr;
	tss_tasks[id].ss0 = GDT_OFF_DS_RING_0 | RPL_LVL_0;

    // Agregar entrada a la gdt
    gdt[nextFreeGDTIndex].limit_0_15 = TSS_SIZE - 1;
    gdt[nextFreeGDTIndex].limit_16_19 = 0x0;
    gdt[nextFreeGDTIndex].base_0_15 = (uint32_t)&(tss_tasks[id]);
    gdt[nextFreeGDTIndex].base_23_16 = (uint32_t)(&(tss_tasks[id])) >> 16;
    gdt[nextFreeGDTIndex].base_31_24 = (uint32_t)(&(tss_tasks[id])) >> 24;
    gdt[nextFreeGDTIndex].type = 9,
    gdt[nextFreeGDTIndex].s = 0;
    gdt[nextFreeGDTIndex].dpl = 0;
    gdt[nextFreeGDTIndex].p = 1;
    gdt[nextFreeGDTIndex].avl = 0;
    gdt[nextFreeGDTIndex].l = 0;
    gdt[nextFreeGDTIndex].db = 1;
    gdt[nextFreeGDTIndex].g = 0;

    nextFreeGDTIndex += 1;
}
