/*
 * G_Isometric.h
 *
 *  Created on: Nov 18, 2023
 *      Author: nds
 */
#pragma once
#include <nds.h>
#include "Constants.h"
#include "P_Util.h"
#include "G_IsoGeneration.h"

void ISO_RenderTiles(s8* world);

void ISO_InitTiles();

bool ISO_isTileFlipped(int tile);
