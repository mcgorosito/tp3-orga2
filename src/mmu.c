/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

// Área libre kernel: 0x100000-0x3fffff
uint32_t proxima_pagina_libre = PRIMERA_PAG_LIBRE;

uint32_t mmu_nextFreeKernelPage() {
    uint32_t pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
    return pagina_libre;
}

/* Mapea la dirección virtual a la dirección física phy, en el esquema de paginación dado por cr3 */
void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrs) {
	// Extraer de la dirección a mapear el directoryIdx y tableIdx
	uint32_t directoryIdx = virtual >> 22;
	uint32_t tableIdx = (virtual >> 12) & 0x3ff;

	// Usando cr3, calcular la dirección de la page directory entry (PDE)
	page_directory_entry *pageDirectory = (page_directory_entry *)(cr3 & 0xFFFFF000);
	page_directory_entry *pageDirectoryEntry = &pageDirectory[directoryIdx];

	// Si el bit de present de la PDE es 0:
	if (!pageDirectoryEntry->present) {
		// Pedir una nueva página para la page table
		page_table_entry *newPageTable = (page_table_entry *)mmu_nextFreeKernelPage();
		// Completarla con ceros
		for (int i = 0; i < 1024; i++) {
			newPageTable[i] = (page_table_entry){0};
		}
		// Completar los atributos y la page table base addr en la PDE
		pageDirectoryEntry->present = MMU_FLAG_PRESENT;
		pageDirectoryEntry->read_write = MMU_FLAG_READWRITE;
		pageDirectoryEntry->user_supervisor = MMU_FLAG_USER;
		pageDirectoryEntry->page_table_base = (uint32_t)newPageTable >> 12;
	}

	// Obtener la page table de la PDE
	page_table_entry *pageTable = (page_table_entry *)(pageDirectoryEntry->page_table_base << 12);
	
	// Usando el puntero a la page table y el campo tableIdx, obtener la page table entry (PTE)
	// Completar los atributos y la physical addr base en la PTE
	pageTable[tableIdx].present = attrs & MASK_PAG_PRESENT;
	pageTable[tableIdx].user_supervisor = (attrs & MASK_PAG_READWRITE) >> 1;
	pageTable[tableIdx].read_write = (attrs & MASK_PAG_USERSUPERVISOR) >> 2;
	pageTable[tableIdx].physical_address_base = ((uint32_t)phy & 0xFFFFF000) >> 12;

	// Optimización: comparar cr3 parámetro con cr3 real para evitar hacer siempre el flush
	if (rcr3() != cr3) {
		tlbflush();
	}
}

/* Desmapea la dirección virtual en el esquema de paginación dado por cr3 */
void mmu_unmapPage(uint32_t cr3, uint32_t virtual) {
    // Extraer de la dirección a mapear el directoryIdx y tableIdx
	uint32_t directoryIdx = virtual >> 22;
	uint32_t tableIdx = (virtual >> 12) & 0x3ff;

	// Usando cr3, calcular la dirección de la page directory entry (PDE)
	page_directory_entry *pageDirectory = (page_directory_entry *)(cr3 & 0xFFFFF000);
	page_directory_entry pageDirectoryEntry = pageDirectory[directoryIdx];

	// Si el bit de present de la PDE es 1
	if (pageDirectoryEntry.present == 1) {
		// Obtener la page table
		page_table_entry *pageTable = (page_table_entry *)(pageDirectoryEntry.page_table_base << 12);
		// Borrar la page table entry
		pageTable[tableIdx] = (page_table_entry){0}; //Alcanza con poner presente en 0
	}
	tlbflush();
}

uint32_t mmu_initKernelDir() {
	page_directory_entry *pageDirectory = (page_directory_entry *)KERNEL_PAGE_DIR;
	page_table_entry *pageTable0 = (page_table_entry *)KERNEL_PAGE_TABLE_0;

	for (int i = 0; i < 1024; i++) {
		pageDirectory[i] = (page_directory_entry){0}; 
		pageTable0[i] = (page_table_entry){0}; 
	}

	pageDirectory[0].present = MMU_FLAG_PRESENT;
	pageDirectory[0].user_supervisor = MMU_FLAG_SUPERVISOR;
	pageDirectory[0].read_write = MMU_FLAG_READWRITE;
	pageDirectory[0].page_table_base = (uint32_t)pageTable0 >> 12;

	for (int i = 0; i < 1024; i++) {
		pageTable0[i].present = MMU_FLAG_PRESENT;
		pageTable0[i].user_supervisor = MMU_FLAG_SUPERVISOR;
		pageTable0[i].read_write = MMU_FLAG_READWRITE;
		pageTable0[i].physical_address_base = i;
	}

    return (uint32_t)pageDirectory;
}

/* Inicializa un directorio de páginas y tablas de páginas para una tarea.
   Mapea las páginas del mundo Cronenberg donde estará ubicada la tarea a
   partir de la dirección virtual 0x08000000(128MB) */
/* NOTA: phy estará entre 0x400000 y 0x1CFFFFF 
   El stack y el código ocupan 8 kb y están solapados */

uint32_t mmu_initTaskDir(uint32_t phy, uint32_t task_code_addr, uint32_t attrs) {
	// Obtener el directory y el table index de la dirección de inicio de la memoria virtual para las tareas
	
	// Inicializar el dir de tablas de páginas y las tablas de páginas
	page_directory_entry *pageDirectory = (page_directory_entry *)mmu_nextFreeKernelPage();

	// Optimización: en vez de mapear el kernel de nuevo, se puede hacer que la entrada 0
	// del directorio apunte a la tabla del kernel
	page_table_entry *kernelTable = (page_table_entry *)KERNEL_PAGE_TABLE_0;
	for (int i = 0; i < 1024; i++) {
		pageDirectory[i] = (page_directory_entry){0};
	}

	// Completar las PDE con los atributos y las page tables base addr
	pageDirectory[0].present = MMU_FLAG_PRESENT;
	pageDirectory[0].user_supervisor = MMU_FLAG_SUPERVISOR;
	pageDirectory[0].read_write = MMU_FLAG_READWRITE;
	pageDirectory[0].page_table_base = (uint32_t)kernelTable >> 12;

	// Mapear las direcciones recibidas por parámetro en la page table de tareas
	mmu_mapPage((uint32_t)pageDirectory, TASK_CODE_INIT, phy, attrs);
	mmu_mapPage((uint32_t)pageDirectory, TASK_CODE_INIT+PAGE_SIZE, phy+PAGE_SIZE, attrs);

	// Mapear al cr3 actual el destino: phy
	// Mapear phy con identity mapping (para no pisar nada)
	// La fuente ya esta en kernel -> mapeada con identity mapping
	mmu_mapPage(rcr3(), phy, phy, 3);
	mmu_mapPage(rcr3(), phy+PAGE_SIZE, phy+PAGE_SIZE, 3);

	// Copiar desde task_code_addr a phy (8 kb)
	uint8_t *fuente = (uint8_t *)task_code_addr;
	uint8_t *destino = (uint8_t *)phy;
	for (int i = 0; i < 8192; i++) {
		destino[i] = fuente[i];
	}

	// Desmapear la dirección destino
	mmu_unmapPage(rcr3(), phy);
	mmu_unmapPage(rcr3(), phy+PAGE_SIZE);

	tlbflush();

	// Devolver el page directory
	return (uint32_t)pageDirectory;

}