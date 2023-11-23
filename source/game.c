/*
 * game.c
 *
 *  Created on: Nov 22, 2023
 *      Author: nds
 */
#include "game.h"
s8 WORLD_MAP[] = {
		1,1,1,1,0,0,0,1,
		1,2,2,1,0,0,0,1,
		1,3,1,1,1,0,0,1,
		1,1,1,1,1,0,0,0,
		1,1,1,1,1,0,1,0,
		0,0,0,0,0,2,2,2,
		0,0,0,0,0,2,2,2,
		0,0,0,0,0,2,2,2,

		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,

		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
};

u8 SCALE = 64;
int TILES_ORIGIN = 15;

void Game_InitScreen()
{
	ISO_RenderTiles(WORLD_MAP);
}

void Game_Refresh(){
	ISO_RenderTiles(WORLD_MAP);
}

void Game_MoveScreen(int x, int y){
	TILES_ORIGIN += 32*y + x;
}

void Game_SetScale(u8 scale){
	SCALE = scale;
}
