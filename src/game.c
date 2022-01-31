/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

uint8_t winner = 0;
uint8_t C137PlayerScore = 0;
uint8_t D248PlayerScore = 0;
uint8_t aliveCronenbergs = 20;

uint8_t rickC137PortalGunUses = 0;
uint8_t rickD248PortalGunUses = 0;
uint8_t rickC137UsedPortalGunInCurrentQuantum = 0;
uint8_t rickD248UsedPortalGunInCurrentQuantum = 0;

portal portalList[4] = { };

uint8_t updateTasks = 0;

void game_init() {
	screen_clearBoard();
	// Inicializar portales en un valor fuera del mapa que significa desactivado
	for (int i = 0; i < 4; i++) {
		portalList[i].x = INACTIVE_PORTAL;
		portalList[i].y = INACTIVE_PORTAL;
	}
	screen_printPlayerScores(C137PlayerScore, D248PlayerScore);
	screen_printCronenbergTable();
	screen_drawTasks();
}

void game_updateScore() {
	if (updateTasks) {
		screen_clearBoard();
		screen_drawTasks();
		screen_printCronenbergTable();
		updateTasks = 0;
	}
	rickC137UsedPortalGunInCurrentQuantum = 0;
	rickD248UsedPortalGunInCurrentQuantum = 0;
	screen_printPlayerScores(C137PlayerScore, D248PlayerScore);
}

void game_killActiveCharacter() {
	taskList[currentTaskId].status = DEAD;
	if (taskList[currentTaskId].type == CronenbergC137) {
		C137PlayerScore--;
		aliveCronenbergs--;
	} else if (taskList[currentTaskId].type == CronenbergD248) {
		D248PlayerScore--;
		aliveCronenbergs--;
	} else if (taskList[currentTaskId].type == Cronenberg) {
		aliveCronenbergs--;
	}
}

uint32_t game_checkEndOfGame() {
	/* El juego termina si:
	- Alguno de los Ricks tiene todas las mentes y el otro ninguna
	- Alguno de los Ricks muere (porque su mente fue capturada o porque lo mataron)
	- Alguno de los Mortys muere
	- NUEVA CONDICIÓN: Alguno de los Ricks o Mortys es capturado por el contrario
	*/
	if (aliveCronenbergs == 0) {
		winner = 0;
		return 1;
	} else if (isDead(0xC137) || isCaptured(0xC137) || aliveCronenbergs == D248PlayerScore) {
   	    winner = 2;
        return 1;
    } else if (isDead(0xD248) || isCaptured(0xD248) || aliveCronenbergs == C137PlayerScore) {
        winner = 1;
        return 1;
    } else {
    	return 0;
    }
}

/* Funciones para syscalls */
uint32_t game_usePortalGun(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty) {
	e_taskType_t currentTaskType = taskList[currentTaskId].type; 
	if (currentTaskType == MortyC137 && rickC137PortalGunUses >= 10) {
		moveCharacter(MORTY_C137_ID, x, y, cross);
		rickC137PortalGunUses = rickC137PortalGunUses - 10;
		return 1;
	}
	if (currentTaskType == MortyD248 && rickD248PortalGunUses >= 10) {
		moveCharacter(MORTY_D248_ID, x, y, cross);
		rickD248PortalGunUses = rickD248PortalGunUses - 10;
		return 1;
	}
	if (currentTaskType == RickC137 && !rickC137UsedPortalGunInCurrentQuantum) {
		moveCharacter(RICK_C137_ID, x, y, cross);
		if (withMorty) {
			// Cambiar al cr3 de Morty
			lcr3(taskList[MORTY_C137_ID].cr3);
			moveCharacter(MORTY_C137_ID, x, y, cross);	
			// Volver al cr3 de Rick
			lcr3(taskList[RICK_C137_ID].cr3);
		}
		rickC137UsedPortalGunInCurrentQuantum = 1;
		return 1;
	}
	if (currentTaskType == RickD248 && !rickD248UsedPortalGunInCurrentQuantum) {
		moveCharacter(RICK_D248_ID, x, y, cross);
		if (withMorty) {
			// Cambiar al cr3 de Morty
			lcr3(taskList[MORTY_D248_ID].cr3);
			moveCharacter(MORTY_D248_ID, x, y, cross);
			// Volver al cr3 de Rick
			lcr3(taskList[RICK_D248_ID].cr3);
		}
		rickD248UsedPortalGunInCurrentQuantum = 1;
		return 1;
	}
	return 0;
}

void game_iAmRick(uint32_t code) {
	e_taskType_t currentTaskType = taskList[currentTaskId].type;
	// Si se da alguna de estas primeras 4 condiciones, termina el juego
	// Se chequea en checkEndOfGame
	if (currentTaskType == RickD248 && code == 0xC137) {
		taskList[currentTaskId].type = RickC137;
	} 
	if (currentTaskType == RickC137 && code == 0xD248) {
		taskList[currentTaskId].type = RickD248;
	}
	if (currentTaskType == MortyD248 && code == 0xC137) {
		taskList[currentTaskId].type = MortyC137;
	} 
	if (currentTaskType == MortyC137 && code == 0xD248) {
		taskList[currentTaskId].type = MortyD248;
	}
	// DECISIÓN: los cronenberg no pueden ser reconquistados
	if (currentTaskType == Cronenberg && code == 0xC137){
		taskList[currentTaskId].type = CronenbergC137;
		C137PlayerScore++;
	}
	if (currentTaskType == Cronenberg && code == 0xD248){
		taskList[currentTaskId].type = CronenbergD248;
		D248PlayerScore++;
	}
}

void game_whereIsMorty(int32_t *x, int32_t *y) {
	if (currentTaskId == RICK_C137_ID) {
		*x = taskList[MORTY_C137_ID].x - taskList[RICK_C137_ID].x;
		*y = taskList[MORTY_C137_ID].y - taskList[RICK_C137_ID].y;
	}
	if (currentTaskId == RICK_D248_ID) {
		*x = taskList[MORTY_D248_ID].x - taskList[RICK_D248_ID].x;
		*y = taskList[MORTY_D248_ID].y - taskList[RICK_D248_ID].y;
	}
}

/* Funciones auxiliares */
void moveCharacter(uint8_t id, int32_t x, int32_t y, uint32_t cross) {
	// Obtener phy destino a partir de actual
	int32_t new_x = (taskList[id].x + x) % SCREEN_COLS;
	if (new_x < 0) {
		new_x = SCREEN_COLS + new_x;
	}
	int32_t new_y = (taskList[id].y + y) % SCREEN_ROWS; 
	if (new_y < 0) {
		new_y = SCREEN_ROWS + new_y;
	}
	uint32_t phyDst = positionToPhyAddr(new_x, new_y);
	phyDst++;
	if (cross) {
		// Mapear la nueva dir física a 0x8002000
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT+2*PAGE_SIZE, phyDst, TASK_ATTRS);
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT+3*PAGE_SIZE, phyDst+PAGE_SIZE, TASK_ATTRS);
		// Copiar el código de 0x8000000 a 0x8002000
		uint8_t *fuente = (uint8_t *)TASK_CODE_INIT;
		uint8_t *destino = (uint8_t *)(TASK_CODE_INIT+2*PAGE_SIZE);
		for (int i = 0; i < 8192; i++) {
			destino[i] = fuente[i];
		}
		// Desmapear 0x8002000
		mmu_unmapPage(taskList[id].cr3, TASK_CODE_INIT+2*PAGE_SIZE);
		mmu_unmapPage(taskList[id].cr3, TASK_CODE_INIT+3*PAGE_SIZE);
		// Desplazar mapeando la nueva dir física (no es necesario desmapear porque se pisa)
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT, phyDst, TASK_ATTRS);
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT+PAGE_SIZE, phyDst+PAGE_SIZE, TASK_ATTRS);
		// Actualizar nueva posición en taskList
		taskList[id].x = new_x;
		taskList[id].y = new_y;
		updateTasks = 1;
	} else {
		// Mapear phy a la dirección 0x8002000
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT+2*PAGE_SIZE, phyDst, TASK_ATTRS);
		mmu_mapPage(taskList[id].cr3, TASK_CODE_INIT+3*PAGE_SIZE, phyDst+PAGE_SIZE, TASK_ATTRS);
		// Actualizar portal en portalList
		portalList[id].x = new_x;
		portalList[id].y = new_y;
		updateTasks = 1;
	}
}

int8_t isDead(uint32_t playerCode) {
	if (playerCode == 0xC137) {
		return taskList[RICK_C137_ID].status == DEAD || taskList[MORTY_C137_ID].status == DEAD;
	} else if (playerCode == 0xD248) {
		return taskList[RICK_D248_ID].status == DEAD || taskList[MORTY_D248_ID].status == DEAD;
	}
	return -1;
}

int8_t isCaptured(uint32_t playerCode) {
	if (playerCode == 0xC137) {
		return taskList[RICK_C137_ID].type == RickD248
			|| taskList[RICK_C137_ID].type == MortyD248
			|| taskList[MORTY_C137_ID].type == RickD248
			|| taskList[MORTY_C137_ID].type == MortyD248; 
	} else if (playerCode == 0xD248) {
		return taskList[RICK_D248_ID].type == RickC137
			|| taskList[RICK_D248_ID].type == MortyC137
			|| taskList[MORTY_D248_ID].type == RickC137
			|| taskList[MORTY_D248_ID].type == MortyC137; 
	}
	return -1; 
}
