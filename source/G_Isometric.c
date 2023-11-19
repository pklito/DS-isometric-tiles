/*
 * G_Isometric.c
 *
 *  In charge of tile graphics
 */
#include "G_Isometric.h"
extern s8* WORLD_MAP;

inline void _setSlice(u16* tiles, int tile, u8 color, TileSlices slice){
	tiles[tile] &= !(0x7 << slice);
	tiles[tile] ^= color << slice;
}

void _GenerateTiles(u16* tiles, s8* world, u8 world_dim_x, u8 world_dim_y, u8 world_dim_z){
	int i,j,k;
	for(k = 0; k < world_dim_z; k++){
		for(j = 0; j < world_dim_y; j++){
			for(i = 0; i < world_dim_x; i++){

				//if air, skip
				if(! world[coords_3d(i,j,k,world_dim_x,world_dim_y)]) continue;
				u8 floor_color = 1;
				u8 wall_color = 2;
				//get topleft tile
				int tile = convertWorldToTile(i,j,k);

				//is this block half shifted down?
				bool is_full = (i+j)%2;
				if(is_full){
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile, floor_color, T_MIDDLE);
					_setSlice(tiles, tile++, wall_color,T_BOTTOM);
					_setSlice(tiles, tile, floor_color, T_MIDDLE);
					_setSlice(tiles, tile, wall_color,T_BOTTOM);

					tile += TILES_SHAPE_WIDTH - 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
					tile += 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
				}
				else{
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile, floor_color, T_BOTTOM);
					_setSlice(tiles, tile, floor_color,T_BOTTOM);

					tile += TILES_SHAPE_WIDTH - 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
					tile += 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
				}
			}
		}
	}
}

void GenerateTiles(u16* tiles){
	_GenerateTiles(tiles, WORLD_MAP, WORLD_DIM_X, WORLD_DIM_Y, WORLD_DIM_Z);
}

void RenderTiles(){
	u16 tiles[TILES_SHAPE_WIDTH * TILES_SHAPE_HEIGHT];
	GenerateTiles(tiles);

}

s16 convertWorldToTile(u8 px, u8 py, u8 pz){
	int tile = TILES_ORIGIN;

	//get the "floor coordinates" equivalent
	s16 x = px - 2*pz;
	s16 y = py - 2*pz;

	//calculate the tile offset from the origin
	int offset_x = (y - x);
	int offset_y = (x+y);
	tile += (offset_x + TILES_SHAPE_WIDTH * offset_y);
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
u16 convertTileToWorld(u16 tile);
