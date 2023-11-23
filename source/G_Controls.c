/*
 * Controls.c
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#include "G_Controls.h"
#include "game.h"
#include <math.h>

int startHeldKeys = -1;

void Game_InitInput(){
}

extern SCALE;
void Game_HandleInput(){
	scanKeys();
	u16 keys = keysHeld();
	u16 keys_pressed = keysDown();
	if(keys_pressed & KEY_RIGHT)
		Game_MoveScreen(1,0);
	if(keys_pressed & KEY_LEFT)
		Game_MoveScreen(-1,0);
	if(keys_pressed & KEY_DOWN)
		Game_MoveScreen(0,1);
	if(keys_pressed & KEY_UP)
		Game_MoveScreen(0,-1);
	if(keys_pressed & KEY_A)
		Game_SetScale((SCALE)%3 + 1);
	if(keys_pressed)
		Game_Refresh();


}
