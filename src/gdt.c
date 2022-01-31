/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[05]  */
        (uint16_t)    0x0000,         /* base[05]   */
        (uint8_t)     0x00,           /* base[236]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[169] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[314]  */
    },

    [GDT_IDX_CS_RING_0] = {
        .limit_0_15 = 0x88ff,       // Limite
        .limit_16_19 = 0x0,
        .base_0_15 = 0,             // Base
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 10,                 // Tipo de segmento (código/datos)
        .s = 1,                     // Flag de sistema (0 = sistema, 1 = no sistema)
        .dpl = 0,                   // Nivel de privilegio 
        .p = 1,                     // Segmento presente
        .avl = 0,                   // Disponible para el sistema, queda en 0
        .l = 0,                     // 32 (l = 0) o 64 bits (l = 1)
        .db = 1,                    // 16 (db = 0) o 32 bits (db = 1)
        .g = 1,                     // Granularidad (0 si el lìmite es en bytes, 1 si es en páginas de 4k)
    },

    [GDT_IDX_CS_RING_3] = {
        .limit_0_15 = 0x88ff,
        .limit_16_19 = 0x0,
        .base_0_15 = 0,
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 10,
        .s = 1,
        .dpl = 3,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1,
    },

    [GDT_IDX_DS_RING_0] = {
        .limit_0_15 = 0x88ff,
        .limit_16_19 = 0x0,
        .base_0_15 = 0,
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 2,
        .s = 1,
        .dpl = 0,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1,
    },

    [GDT_IDX_DS_RING_3] = {
        .limit_0_15 = 0x88ff,
        .limit_16_19 = 0x0,
        .base_0_15 = 0,
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 2,
        .s = 1,
        .dpl = 3,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1,
    },

    [GDT_IDX_VM_RING_0] = {
        .limit_0_15 = 0x1f3f,
        .limit_16_19 = 0x0,
        .base_0_15 = 0x8000,
        .base_23_16 = 0x0b,
        .base_31_24 = 0x00,
        .type = 2,
        .s = 1,
        .dpl = 0,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0,
    },

    [GDT_IDX_TSS_INITIAL] = {
        .limit_0_15 = TSS_SIZE - 1,
        .limit_16_19 = 0x0,
        .base_0_15 = 0,
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 9,
        .s = 0,
        .dpl = 0,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0,
    },

    [GDT_IDX_TSS_IDLE] = {
        .limit_0_15 = TSS_SIZE - 1,
        .limit_16_19 = 0x0,
        .base_0_15 = 0,
        .base_23_16 = 0,
        .base_31_24 = 0,
        .type = 9,
        .s = 0,
        .dpl = 0,
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0,
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
