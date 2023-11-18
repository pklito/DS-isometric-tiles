/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "Controls.h"
#include <math.h>

int startHeldKeys = -1;

void initInput(){
}

void handleInput(){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();
}
