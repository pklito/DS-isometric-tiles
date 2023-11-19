/*
 * G_Isometric.c
 *
 *  In charge of tile graphics
 */
#include "G_Isometric.h"

inline void _setSlice(u16* tiles, int tile, u8 color, TileSlices slice){
	printf("set %d,%d,%d\n",tile,color,slice);
	tiles[tile] &= !(0x7 << slice);
	tiles[tile] ^= color << slice;
}
void ISO_GenerateTiles(u16* tiles, s8* world, u8 world_dim_x, u8 world_dim_y, u8 world_dim_z){
	int i,j,k;
	printf("%p\n",world);
	for(k = 0; k < world_dim_z; k++){
		for(j = 0; j < world_dim_y; j++){
			for(i = 0; i < world_dim_x; i++){

				//if air, skip

				if(! world[coords_3d(i,j,k,world_dim_x,world_dim_y)]) continue;
				printf("%d,%d,%d, %d:%d\n",i,j,k, coords_3d(i,j,k,world_dim_x,world_dim_y),world[coords_3d(i,j,k,world_dim_x,world_dim_y)]);
				u8 floor_color = 1;
				u8 wall_color = 2;
				//get topleft tile
				int tile = ISO_convertWorldToTile(i,j,k);

				//is this block half shifted down?
				bool is_full = ((i+j)%2 == 0);
				if(is_full){
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile, floor_color, T_MIDDLE);
					_setSlice(tiles, tile++, wall_color,T_BOTTOM);
					_setSlice(tiles, tile, floor_color, T_MIDDLE);
					_setSlice(tiles, tile, wall_color,T_BOTTOM);

					tile += TILES_SHAPE_WIDTH - 1;

					_setSlice(tiles, tile, wall_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
					tile += 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
				}
				else{
					//TODO edge case of wrapping, edge case of original tile wrapping
					_setSlice(tiles, tile++, floor_color, T_BOTTOM);
					_setSlice(tiles, tile, floor_color,T_BOTTOM);

					tile += TILES_SHAPE_WIDTH - 1;
					_setSlice(tiles, tile, floor_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
					_setSlice(tiles, tile, wall_color, T_BOTTOM);
					tile += 1;
					_setSlice(tiles, tile, floor_color, T_TOP);
					_setSlice(tiles, tile, wall_color, T_MIDDLE);
					_setSlice(tiles, tile, wall_color, T_BOTTOM);
					tile += TILES_SHAPE_WIDTH - 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
					tile += 1;
					_setSlice(tiles, tile, wall_color, T_TOP);
				}
			}
		}
	}
}

u16 tiles[TILES_SHAPE_WIDTH * TILES_SHAPE_HEIGHT];
void ISO_RenderTiles(s8* world){
	ISO_GenerateTiles(tiles, world, WORLD_DIM_X, WORLD_DIM_Y, WORLD_DIM_Z);
	int i,j;
	for(j = 0; j < TILES_SHAPE_WIDTH*TILES_SHAPE_HEIGHT/2; j++){
			int tile = tiles[j];
			u8 bottom = tile & 0xf;
			u8 middle = (tile & 0xf0) >> 4;
			u8 top = (tile & 0xf00) >> 8;
			if(tile){
				if(bottom == middle && middle == top){
					//solid color
					BG_MAP_RAM(1)[j] &= 0 | (middle << 12) | (j%2 == 0 ? BIT(10) : 0);
				}
				else{
					BG_MAP_RAM(1)[j] = 0 | (middle << 12) | ((bottom) << 14) | (j%2 == 0 ? BIT(10) : 0);
				}
			}
	}
}

s16 ISO_convertWorldToTile(u8 px, u8 py, u8 pz){
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
u16 ISO_convertTileToWorld(u16 tile);


/* TILES */

u8 TILE_FULL[] = {
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00
};


u8 TILE_ABC_F1F2F3[] = {
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x33,0x22,0x22,0x22,
		0x33,0x33,0x22,0x22,
		0x33,0x33,0x33,0x22,
		0x33,0x33,0x33,0x33,
};
u8 TILE_ABC_F1F2W2[] = {
		0x22,0x22,0x22,0x44,
		0x22,0x22,0x44,0x44,
		0x22,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55,
};
u8 TILE_ABC_F1W1F2[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x22,0x55,0x55,0x55,
		0x22,0x22,0x55,0x55,
		0x22,0x22,0x22,0x55,
		0x22,0x22,0x22,0x22,
};
u8 TILE_ABC_WFF_W2F2F3[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x22,0x44,0x44,0x44,
		0x22,0x22,0x44,0x44,
		0x22,0x22,0x22,0x44,
		0x22,0x22,0x22,0x22,
};
u8 TILE_ABC_WFF_W3F2F3[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x44,0x22,0x22,0x22,
		0x44,0x44,0x22,0x22,
		0x44,0x44,0x44,0x22,
		0x44,0x44,0x44,0x44,
};
u8 TILE_ABC_WFF_W1F2F3[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x33,0x22,0x22,0x22,
		0x33,0x33,0x22,0x22,
		0x33,0x33,0x33,0x22,
		0x33,0x33,0x33,0x33,
};
/*Two are same*/
u8 TILE_ABA_F1F2F1[] = {
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x11,0x11,
};
u8 TILE_ABA_F1W1F1[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x44,0x44,
};
u8 TILE_ABA_W1F1W1[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55,
};

u8 TILE_AAB_F1F1F2[] = {
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x22,0x11,0x11,0x11,
		0x22,0x22,0x11,0x11,
		0x22,0x22,0x22,0x11,
		0x22,0x22,0x22,0x22,
};

u8 TILE_AAB_F1F1W1[] = {
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55,
};

u8 TILE_AAB_WWX_W1W1F2[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x22,0x55,0x55,0x55,
		0x22,0x22,0x55,0x55,
		0x22,0x22,0x22,0x55,
		0x22,0x22,0x22,0x22,
};
u8 TILE_AAB_WWX_W1W1W2[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x66,0x55,0x55,0x55,
		0x66,0x66,0x55,0x55,
		0x66,0x66,0x66,0x55,
		0x66,0x66,0x66,0x66,
};

u8 TILE_AAB_WWX_W1W1W1B[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x11,0x55,0x55,0x55,
		0x11,0x11,0x55,0x55,
		0x11,0x11,0x11,0x55,
		0x11,0x11,0x11,0x11,
};

u8 TILE_AAB_WWX_W1BW1BW1[] = {
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x55,0x11,0x11,0x11,
		0x55,0x55,0x11,0x11,
		0x55,0x55,0x55,0x11,
		0x55,0x55,0x55,0x55,
};

u8 TILE_ABB_F1W1W1[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
};

u8 TILE_ABB_F1F2F2[] = {
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
};

u8 TILE_ABB_WFF_W2F2F2[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
};

u8 TILE_ABB_WFF_W1F2F2[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
};

u16 COLOR_PALETTE[] = {
		COLOR_WATER,COLOR_F1, COLOR_F2, COLOR_FS, COLOR_W1A,COLOR_W1B,COLOR_W2A,COLOR_W2B,
		COLOR_F1,	COLOR_F1, COLOR_F2, COLOR_F2, COLOR_FS, COLOR_FS, COLOR_W1A,COLOR_W2A,
		COLOR_F2, 	COLOR_FS, COLOR_F1, COLOR_FS, COLOR_F1, COLOR_F2, COLOR_F2, COLOR_FS,
		COLOR_FS, 	COLOR_F2, COLOR_FS, COLOR_F1, COLOR_F2, COLOR_F1, COLOR_FS, COLOR_F1,
		COLOR_F1,	COLOR_F1, COLOR_F2, COLOR_F2, COLOR_F2, COLOR_F1, COLOR_F1, COLOR_F2,
		COLOR_W1A, 	COLOR_W1A,COLOR_W2A,COLOR_W2A,COLOR_W2B,COLOR_W1B,COLOR_W1B,COLOR_W2B,
		COLOR_W2A,  COLOR_W2B,COLOR_W1A,COLOR_W1B,COLOR_W1A,COLOR_W2B,COLOR_W2A,COLOR_W1B,
		COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER
};
void ISO_InitTiles(){
	/* Palette */
	u8 color_offset = 0;
	u8 palette_offset = 0;
	//create the manual list with loops to not have to change the offsets every time.
	for(color_offset = 0; color_offset < 8; color_offset ++){
		for(palette_offset = 0; palette_offset < 8; palette_offset++){
			BG_PALETTE[coords(color_offset,palette_offset,16)] = COLOR_PALETTE[coords(palette_offset,color_offset,8)];
		}
	}
	//offset 1

	/* Tiles */
	dmaCopy(TILE_FULL, &BG_TILE_RAM(0)[T_FULL<<4], 32);
	dmaCopy(TILE_ABC_F1F2F3, &BG_TILE_RAM(0)[T_ABC_F1F2F3<<4], 32);
}
