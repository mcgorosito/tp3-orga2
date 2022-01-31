/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef struct portalInfo { // creatorId es la posición en el array (0, 1, 2, 3)
	uint8_t x;
	uint8_t y;
} __attribute__((__packed__, aligned (8))) portal;


/* -------------------------------------------------------------------------- */
/* Variables globales del juego */
// Score de los jugadores (cantidad de habitantes Cronenberg asimilados por cada Rick)
extern uint8_t C137PlayerScore;
extern uint8_t D248PlayerScore;
extern uint8_t aliveCronenbergs;
extern uint8_t winner;

// Stats de uso de Portal Gun
extern uint8_t rickC137PortalGunUses;
extern uint8_t rickD248PortalGunUses;
extern uint8_t rickC137UsedPortalGunInCurrentQuantum;
extern uint8_t rickD248UsedPortalGunInCurrentQuantum;

// Posiciones de los portales
extern portal portalList[4];

// Flag para actualizar pantalla
extern uint8_t updateTasks;
/* -------------------------------------------------------------------------- */


void game_init();
void game_updateScore();
void game_killActiveCharacter();
uint32_t game_checkEndOfGame();

/* Funciones para syscalls */
uint32_t game_usePortalGun(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty);
void game_iAmRick(uint32_t code);
void game_whereIsMorty(int32_t *x, int32_t *y);

/* Funciones auxiliares */
void moveCharacter(uint8_t id, int32_t x, int32_t y, uint32_t cross);
int8_t isDead(uint32_t playerCode);
int8_t isCaptured(uint32_t playerCode);

#endif  /* !__GAME_H__ */
