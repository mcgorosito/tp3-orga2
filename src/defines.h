/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
/* Masks */
#define MASK_PAG_PRESENT        0x00000001
#define MASK_PAG_READWRITE      0x00000002
#define MASK_PAG_USERSUPERVISOR 0x00000004

/* Flags*/
#define MMU_FLAG_PRESENT 1
#define MMU_FLAG_SUPERVISOR 0
#define MMU_FLAG_USER 1
#define MMU_FLAG_READ 0
#define MMU_FLAG_READWRITE 1

#define TASK_ATTRS 7

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  40 // X
#define SIZE_M                  80 // Y
#define PAGE_SIZE               1024*4 // 4 kb
#define TASK_CODE_SIZE          1024*8 // 8 kb
#define TSS_SIZE                104

/* Definicion de la pantalla */
/* -------------------------------------------------------------------------- */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 50

#define GDT_IDX_NULL_DESC       0
#define GDT_IDX_CS_RING_0       8
#define GDT_IDX_CS_RING_3       9
#define GDT_IDX_DS_RING_0       10
#define GDT_IDX_DS_RING_3       11
#define GDT_IDX_VM_RING_0       12
#define GDT_IDX_TSS_INITIAL     13
#define GDT_IDX_TSS_IDLE        14

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC       (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_CS_RING_0       (GDT_IDX_CS_RING_0 << 3)
#define GDT_OFF_CS_RING_3       (GDT_IDX_CS_RING_3 << 3)
#define GDT_OFF_DS_RING_0       (GDT_IDX_DS_RING_0 << 3)
#define GDT_OFF_DS_RING_3       (GDT_IDX_DS_RING_3 << 3)
#define GDT_OFF_VM_RING_0       (GDT_IDX_VM_RING_0 << 3)
#define GDT_OFF_TSS_INITIAL     (GDT_IDX_TSS_INITIAL << 3)
#define GDT_OFF_TSS_IDLE        (GDT_IDX_TSS_IDLE << 3)

/* Defines para la IDT */
/* -------------------------------------------------------------------------- */
#define INTERRUPT_0_ATTRS 0x8e00
#define INTERRUPT_3_ATTRS 0xee00

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
#define COD_SEL_0 (0x08 << 3)

/* TSS */
/* -------------------------------------------------------------------------- */
#define DEFAULT_EFLAGS 2
#define EFLAGS_WITH_INT 0x202
#define RPL_LVL_0 0x0
#define RPL_LVL_3 0x3
#define IO_MAP_DISABLED 0xffff

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE_INIT           0x08000000 /* direccion virtual del codigo */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define RICK_C137_CODE           0x10000
#define MORTY_C137_CODE          0x12000
#define RICK_D248_CODE           0x14000
#define MORTY_D248_CODE          0x16000
#define CRONENBERG_CODE          0x18000
#define IDLE                     0x1A000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_STACK_BASE        0x00027000
#define KERNEL_PAGE_DIR          0x00027000
#define KERNEL_PAGE_TABLE_0      0x00028000
#define CRYSTALS_MAP             0x00029000
#define PRIMERA_PAG_LIBRE		 0x100000

/* Game */
/* -------------------------------------------------------------------------- */
#define CRONENBERG_WORLD_START   0x0400000
#define CRONENBERG_WORLD_END     0x1CFFFFF

/* Character id */
#define RICK_C137_ID   0
#define MORTY_C137_ID  1
#define RICK_D248_ID   2
#define MORTY_D248_ID  3

/* Statuses */
#define ALIVE 1
#define DEAD 0
#define INACTIVE_PORTAL 80

/* Screen */
#define SCREEN_ROWS 40
#define SCREEN_COLS 80
#define DEAD_CHAR x
#define RED_CHAR r
#define BLUE_CHAR b
#define BROWN_CHAR |

#endif  /* !__DEFINES_H__ */
