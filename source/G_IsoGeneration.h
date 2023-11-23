/*
 * G_Isometric_generation.h
 *
 *  Created on: Nov 22, 2023
 *      Author: nds
 */
#pragma once
#include <nds.h>
#include "Constants.h"
#include "P_Util.h"

typedef enum {
	T_BOTTOM = 0,
	T_MIDDLE = 5,
	T_TOP    = 10
} TileSlices;

#define SLICE_MASK 0b11111

typedef enum {
	/*SAME COLOR*/
	T_FULL,
	/*2 COLORS*/
	T_ABA_F1F2F1,
	T_ABA_F1W1F1,
	T_ABA_W1F1W1,

	T_AAB_F1F1F2,
	T_AAB_DDF2,	//water
	T_AAB_F1F1W1,
	T_AAB_WWX_W1W1F2,
	T_AAB_WWX_W1W1D,	//water
	T_AAB_WWX_W1W1F1,
	T_AAB_WWX_W1W1W2,
	T_AAB_WWX_W1W1W1B,
	T_AAB_WWX_W1BW1BW1,

	T_ABB_F1W1W1,
	T_ABB_F1F2F2,
	T_ABB_WFF_W1DD,	//water
	T_ABB_WFF_W2F2F2,
	T_ABB_WFF_W1F2F2,
	T_ABB_DF2F2,	//water

	/* THREE COLORS*/
	T_ABC_F1F2F3,
	T_ABC_DF2F3,	//water
	T_ABC_F1F2W2,
	T_ABC_DF2W2,	//water
	T_ABC_F1W1F2,
	T_ABC_W1F2W2,
	T_ABC_W2F2W2B,
	T_ABC_W2BF2W2,
	T_ABC_WFF_W2F2F3,
	T_ABC_WFF_W3F2F3,
	T_ABC_WFF_W1F2F3,
	T_ABC_WFF_W1DF3,
	T_ABC_WFF_W1DF1


} TileTypes;

void ISO_GenerateTiles(u16* tiles, s8* world, u8 world_dim_x, u8 world_dim_y, u8 world_dim_z);
void ISO_GenerateTilesInverse(u16* tiles);
/*
 * Return tile number, in 32x32 world.
 *
 */
s16 ISO_convertWorldToTile(u8 px, u8 py, u8 pz,s8* cull_lr);

/*
 * Gives the bottom left solution.
 * return = (px) | (py << 8);  //pz = 0
 * or return = U16_MAX in case of failure
 *
 * returns the lowest tile affecting this one (the bottom triangle)
 *
 */
u16 ISO_convertTileToWorld(u16 tile);


