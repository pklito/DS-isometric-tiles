/*
 * G_IsoGeneration.c
 *
 *  Created on: Nov 22, 2023
 *      Author: nds
 */
#include "G_IsoGeneration.h"
typedef enum {
	FACE_FLOOR = 0,
	FACE_WALL_LEFT = 0b01,
	FACE_WALL_RIGHT = 0b10
} BlockFaces;

inline void _setSlice(u16* tiles, int tile, TileSlices slice, u8 color,BlockFaces face){
	tiles[tile] &= ~((SLICE_MASK << slice) | BIT(15));
	tiles[tile] ^= ((face<<3) | color) << slice | BIT(15);
}
inline void _setWhole(u16* tiles, int tile, u8 color_top, BlockFaces face_top, u8 color_middle, BlockFaces face_middle, u8 color_bottom, BlockFaces face_bottom){
	tiles[tile] = BIT(15) | ((face_bottom<<3|color_bottom)<<T_BOTTOM) | ((face_middle<<3|color_middle)<<T_MIDDLE)| ((face_top<<3|color_top)<<T_TOP) ;
}


void ISO_GenerateTiles(u16* tiles, s8* world, u8 world_dim_x, u8 world_dim_y, u8 world_dim_z){
	int i,j,k;
	u8 scale = 3;

	for(k = 0; k < world_dim_z * scale; k++){
		for(j = 0; j < world_dim_y * scale; j++){
			for(i = 0; i < world_dim_x * scale; i++){

				//if air, skip
				u8 block_type = world[coords_3d(i/scale,j/scale,k/scale,world_dim_x,world_dim_y)];
				if(! block_type) continue;

				//get topleft tile
				int tile = ISO_convertWorldToTile(i,j,k);

				//is this block half shifted down?
				bool is_full = ((i+j)%2 == 0);
				if(is_full){
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile, T_MIDDLE, block_type, FACE_FLOOR);
					_setSlice(tiles, tile++,T_BOTTOM, block_type,FACE_WALL_LEFT);
					_setSlice(tiles, tile, T_MIDDLE, block_type, FACE_FLOOR);
					_setSlice(tiles, tile, T_BOTTOM, block_type, FACE_WALL_RIGHT);

					tile += TILES_SHAPE_WIDTH - 1;

					_setSlice(tiles, tile,T_TOP, block_type, FACE_WALL_LEFT);
					_setSlice(tiles, tile,T_MIDDLE, block_type, FACE_WALL_LEFT);
					tile += 1;
					_setSlice(tiles, tile,T_TOP, block_type, FACE_WALL_RIGHT);
					_setSlice(tiles, tile,T_MIDDLE, block_type, FACE_WALL_RIGHT);
				}
				else{
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile++, T_BOTTOM, block_type, FACE_FLOOR); 	//++!
					_setSlice(tiles, tile, T_BOTTOM, block_type, FACE_FLOOR);

					tile += TILES_SHAPE_WIDTH - 1;
					_setWhole(tiles,tile,block_type, FACE_FLOOR, block_type, FACE_WALL_LEFT, block_type, FACE_WALL_LEFT);
					tile += 1;
					_setWhole(tiles,tile,block_type, FACE_FLOOR, block_type, FACE_WALL_RIGHT, block_type, FACE_WALL_RIGHT);
					tile += TILES_SHAPE_WIDTH - 1;
					_setSlice(tiles, tile, T_TOP, block_type, FACE_WALL_LEFT);
					tile += 1;
					_setSlice(tiles, tile, T_TOP, block_type, FACE_WALL_RIGHT);
				}
			}
		}
	}
}

