/*
 * game.h
 *
 *  Created on: Nov 22, 2023
 *      Author: nds
 */
#pragma once
#include <nds.h>
#include "G_IsoRender.h"
void Game_InitScreen();

void Game_Refresh();

void Game_MoveScreen(int x, int y);
