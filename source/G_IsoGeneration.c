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
	u8 scale = 4;

	for(k = 0; k < world_dim_z * scale; k++){
		for(j = 0; j < world_dim_y * scale; j++){
			for(i = 0; i < world_dim_x * scale; i++){

				//if air, skip
				u8 block_type = world[coords_3d(i/scale,j/scale,k/scale,world_dim_x,world_dim_y)];
				if(! block_type) continue;

				//get topleft tile
				int tile = ISO_convertWorldToTile(i,j,k);

				//this block goes off screen
				if(tile == -1) continue;
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

s16 ISO_convertWorldToTile(u8 px, u8 py, u8 pz){
	int tile = TILES_ORIGIN;

	//get the "floor coordinates" equivalent

	s16 x = (s8)px;
	s16 y = (s8)py;


	//calculate the tile offset from the origin
	int offset_x = (y - x);
	int offset_y = (x+y)/2 - pz;
	if(tile%TILES_SHAPE_WIDTH + offset_x < 0 || tile%TILES_SHAPE_WIDTH + offset_x >= TILES_SHAPE_WIDTH) return -1;
	tile += (offset_x + TILES_SHAPE_WIDTH * offset_y);
	if(tile < 0 || tile/TILES_SHAPE_WIDTH >= TILES_SHAPE_HEIGHT) return -1;
	return tile;
}

/*
 * Gives the bottom left solution.
 * return = (px) | (py << 8);  //pz = 0
 * or return = U16_MAX in case of failure
 *
 * returns the lowest tile affecting this one (the bottom triangle)
 *
 */
u16 ISO_convertTileToWorld(u16 tile){

}
