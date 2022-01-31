/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"

typedef struct str_tss {
    uint16_t  ptl;        // Previous Task Link
    uint16_t  unused0;
    uint32_t  esp0;       // Stack pointer de la pila de nivel 0
    uint16_t  ss0;        // Stack segment de nivel 0
    uint16_t  unused1;
    uint32_t  esp1;       // Stack pointer de la pila de nivel 1
    uint16_t  ss1;        // Stack segment de nivel 1
    uint16_t  unused2;
    uint32_t  esp2;       // Stack pointer de la pila de nivel 2
    uint16_t  ss2;        // Stack segment de nivel 2
    uint16_t  unused3;
    uint32_t  cr3;
    uint32_t  eip;        // Instruction pointer
    uint32_t  eflags;     // Flags y registros de propósito general
    uint32_t  eax;
    uint32_t  ecx;
    uint32_t  edx;
    uint32_t  ebx;
    uint32_t  esp;
    uint32_t  ebp;
    uint32_t  esi;
    uint32_t  edi;
    uint16_t  es;         // Selectores de segmento
    uint16_t  unused4;
    uint16_t  cs;
    uint16_t  unused5;
    uint16_t  ss;
    uint16_t  unused6;
    uint16_t  ds;
    uint16_t  unused7;
    uint16_t  fs;
    uint16_t  unused8;
    uint16_t  gs;
    uint16_t  unused9;
    uint16_t  ldt;        // LDT Segment Selector
    uint16_t  unused10;
    uint16_t  dtrap;      // Bit T
    uint16_t  iomap;      // I/O Map Base Address
} __attribute__((__packed__, aligned (8))) tss;

extern tss tss_initial;
extern tss tss_idle;
extern uint8_t nextFreeGDTIndex;

void tss_init(uint32_t task_cr3, uint8_t task_id);
void tss_init_initial();
void tss_init_idle();


#endif  /* !__TSS_H__ */
