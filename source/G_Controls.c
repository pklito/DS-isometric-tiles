/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "G_Controls.h"
#include <math.h>

int startHeldKeys = -1;

void Game_InitInput(){
}

void Game_HandleInput(){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();
}
