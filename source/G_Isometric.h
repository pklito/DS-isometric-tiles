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

typedef enum {
	T_BOTTOM = 0,
	T_MIDDLE = 4,
	T_TOP    = 8
} TileSlices;
/*
 *
 */
void GenerateTiles(u16* tiles);
void GenerateTilesInverse(u16* tiles);
/*
 * Return tile number, in 32x32 world.
 *
 */
s16 convertWorldToTile(u8 px, u8 py, u8 pz);

/*
 * Gives the bottom left solution.
 * return = (px) | (py << 8);  //pz = 0
 * or return = U16_MAX in case of failure
 *
 * returns the lowest tile affecting this one (the bottom triangle)
 *
 */
u16 convertTileToWorld(u16 tile);
