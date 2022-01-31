/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};

#define IDT_ENTRY(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) COD_SEL_0;                                                     \
    idt[numero].attr = (uint16_t) INTERRUPT_0_ATTRS;                                               \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

void idt_init() {
    IDT_ENTRY(0);   // Divide by Zero
    IDT_ENTRY(1);   // Debug
    IDT_ENTRY(2);   // Non-Maskable Interrupt
    IDT_ENTRY(3);   // Breakpoint
    IDT_ENTRY(4);   // Overflow
    IDT_ENTRY(5);   // Bound Range Exceeded
    IDT_ENTRY(6);   // Invalid Opcode
    IDT_ENTRY(7);   // Device Not Avaiable
    IDT_ENTRY(8);   // Double Fault
    IDT_ENTRY(9);   // Coprocessor Segment Overrun (reserved)
    IDT_ENTRY(10);  // Invalid TTS
    IDT_ENTRY(11);  // Segment not present
    IDT_ENTRY(12);  // Stack Segment Fault
    IDT_ENTRY(13);  // General Protection Fault
    IDT_ENTRY(14);  // Page Fault
    // 15 -> Reserved
    IDT_ENTRY(16);  // x87 Floating-Point exception
    IDT_ENTRY(17);  // Alignment check
    IDT_ENTRY(18);  // Machine check
    IDT_ENTRY(19);  // SIMD Floating Point Exception
    // 20 - 31 -> Reserved
    // 32 - 255 -> User defined
    IDT_ENTRY(32);  // Clock
    IDT_ENTRY(33);  // Keyboard
    IDT_ENTRY(137); // Syscalls
    idt[137].attr = (uint16_t) INTERRUPT_3_ATTRS;
    IDT_ENTRY(138);
    idt[138].attr = (uint16_t) INTERRUPT_3_ATTRS;
    IDT_ENTRY(139);
    idt[139].attr = (uint16_t) INTERRUPT_3_ATTRS;
}
