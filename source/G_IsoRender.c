/*
 * G_Isometric.c
 *
 *  In charge of tile graphics
 */
#include <stdio.h>
#include "G_IsoRender.h"

inline bool _isFloor(u8 color){
	return (color & 0b11000) == 0;
}
inline bool _isSameBlock(u8 color1, u8 color2){
	return (color1 & 0b111) == (color2 & 0b111);
}
inline bool _isWater(u8 color){
	return color == 0;
}
inline bool _isLeftWall(u8 color){
	return (color&0b11000)>>3 == 1;
}
inline bool _isRightWall(u8 color){
	return (color&0b11000)>>3 == 2;
}
inline int _p_f1_f2(u8 primary_color, u8 secondary_color){
	return 2 * (primary_color-1) + (2*secondary_color > (5-primary_color));
}
inline int _p_w1(u8 floor_color, u8 wall_type){
	return 4 * (wall_type-1) + 2 * (floor_color-1) + (floor_color==1);
}
inline int _p_w1_f2(u8 wall_color, u8 wall_type, u8 other_floor_color){
	if(wall_type==1)
		return _p_f1_f2(wall_color,other_floor_color);	//first 4 rows color choice
	else
		return 4 + (wall_color==1) + 2 *(other_floor_color==3);
}
inline int _p_w1_w2(u8 wall_color,u8 wall_type, u8 other_wall_type){	//wall: row 5, other_wall: row 6
	return (wall_type==2 ? 4 + (2-wall_color) + 2*(other_wall_type==2) : (wall_color-1) + 2*(other_wall_type==2));
}
int _paletteFinder(TileTypes tile_type, u8 bottom, u8 middle, u8 top){
	u8 bot_color = bottom & 0b00111;
	u8 bot_face = (bottom & 0b11000) >> 3;
	u8 mid_color = middle & 0b00111;
	u8 mid_face = (middle & 0b11000) >> 3;	//thats you, reading this
	u8 top_color = top & 0b00111;
	u8 top_face = (top & 0b11000) >> 3;
	switch(tile_type){
	case T_FULL:
		return _isFloor(bottom) ? bottom : 4+(bot_face-1 + 2*(bot_color-1));
		//One floor alone
	case T_AAB_DDF2:	//001
	case T_ABB_DF2F2:	//011
		return 2 * (bot_color-1);
		//Two floors	(mid,bot)
	case T_AAB_F1F1F2:	//112
	case T_ABC_DF2F3:	//012
		return _p_f1_f2(mid_color,bot_color);//
		//Two floors (top, mid)
	case T_ABA_F1F2F1:	//121
	case T_ABB_F1F2F2:	//122
	case T_ABC_F1F2F3:	//123
		return _p_f1_f2(top_color,mid_color);
		//Floor and its wall
	case T_AAB_F1F1W1:	//445
	case T_ABA_W1F1W1:	//545
	case T_ABB_F1W1W1:	//455
	case T_ABC_DF2W2:	//045
		return _p_w1(bot_color,bot_face);			//4 4 5	 if left wall, F1 F1 F2 F2, (4 + F2 F1 F1 F2)
		//Floor and it's top wall
	case T_ABB_WFF_W2F2F2:	//544
	case T_AAB_WWX_W1W1F1:	//554
	case T_ABC_WFF_W1DF1:
		return _p_w1(top_color,top_face);
		//Floor and it's mid wall
	case T_ABA_F1W1F1:	//454
		return _p_w1(mid_color,mid_face);

		//One wall (top)
	case T_AAB_WWX_W1W1D:	//550
	case T_ABB_WFF_W1DD:	//500
		return _p_w1(top_color,top_face);

		//Wall and opposing floor
	case T_AAB_WWX_W1W1F2:	//552
	case T_ABB_WFF_W1F2F2:	//522
	case T_ABC_WFF_W2F2F3:	//542
	case T_ABC_WFF_W1DF3:
		return _p_w1_f2(top_color,top_face,bot_color);
	case T_ABC_F1F2W2:		//245
		return _p_w1_f2(bot_color,bot_face,top_color);
	case T_ABC_WFF_W3F2F3:	//524
	case T_ABC_WFF_W1F2F3:	//523S
		return _p_w1_f2(top_color,top_face,mid_color);
	case T_ABC_F1W1F2:
		return _p_w1_f2(mid_color, mid_face, bot_color);

		//Both wall colors (extra bit for the side of wallB)
	case T_AAB_WWX_W1W1W2:	//556
		return _p_w1_w2(mid_color,mid_face,bot_face);
	case T_ABC_W1F2W2:
		return _p_w1_w2(bot_color,bot_face,top_face);
		//(edge case)
	case T_AAB_WWX_W1BW1BW1:
	case T_AAB_WWX_W1W1W1B:
	case T_ABC_W2BF2W2:
	case T_ABC_W2F2W2B:
		return 8 - bot_color;
	/* Three colors */





	//printf("%d - %d %d %d %d %d %d\n",4 * (bot_face-1) + ((2 * mid_color)+!(bot_face==1))%4, bot_color,bot_face,mid_color,mid_face,top_color,top_face);

	}

	return 10;
}
extern int TILES_ORIGIN;
inline bool ISO_isTileFlipped(int tile){
	return (tile%2) != ((TILES_ORIGIN*TILES_ORIGIN)%2);
}
u16 tiles[TILES_SHAPE_WIDTH * TILES_SHAPE_HEIGHT];
void ISO_RenderTiles(s8* world){
	memset(tiles,0,2*TILES_SHAPE_HEIGHT*TILES_SHAPE_WIDTH);
	ISO_GenerateTiles(tiles, world, WORLD_DIM_X, WORLD_DIM_Y, WORLD_DIM_Z);
	int j;
	for(j = 0; j < TILES_SHAPE_WIDTH*TILES_SHAPE_HEIGHT/1; j++){
		int tile = tiles[j];
		u8 bottom = (tile & (SLICE_MASK<<T_BOTTOM))>> T_BOTTOM;
		u8 middle = (tile & (SLICE_MASK<<T_MIDDLE)) >> T_MIDDLE;
		u8 top = (tile & (SLICE_MASK<<T_TOP)) >> T_TOP;
		if(!tile){
			BG_MAP_RAM(3)[j] = T_FULL | BIT(10);
			continue;
		}

		//How many triangles are the same? (This is how i split the tiles)
		int unique_colors = 3 - ((bottom==middle) + (middle==top||top==bottom));
			//printf("(%d,%d) %d %d %d, %d\n",j%32,j/32,bottom, middle, top,tile);
		//I flip every other tile to create the diamond shapes.
		u16 isFlipped = (ISO_isTileFlipped(j) ? BIT(10) : 0);
		byte palette = 0xff;
		u16 tile_index = 10;
		switch(unique_colors){
		case 1:
			//Check if the solid color is a wall or not. see COLOR_PALETTE first row
			palette = _paletteFinder(T_FULL, bottom, middle, top);
			BG_MAP_RAM(3)[j] = isFlipped | T_FULL | palette<<12;
			break;
		case 2:
			/* ABA */
			if(top == bottom){
				if(_isFloor(top) && _isFloor(middle) && _isFloor(bottom)){	//F1F2F1
					tile_index = T_ABA_F1F2F1;
					palette = _paletteFinder(T_ABA_F1F2F1, bottom,middle,top);
				}
				else if(_isFloor(top) && !_isFloor(middle) && _isFloor(bottom) && _isSameBlock(top,middle)){	//F1W1F1
					tile_index = T_ABA_F1W1F1;
					palette = _paletteFinder(T_ABA_F1W1F1, bottom,middle,top);
				}
				else if(!_isFloor(top) && _isFloor(middle) && !_isFloor(bottom) && _isSameBlock(top,middle)){	//W1F1W1
					tile_index = T_ABA_W1F1W1;
					palette = _paletteFinder(T_ABA_W1F1W1, bottom,middle,top);
				}
				else{
					printf("ERROR - pattern: ABA\n %d, %x,%x,%x\n",j, bottom, middle, top);
				}
			}
			/* AAB */
			else if(top == middle){
				if(_isFloor(top) && _isFloor(middle) && _isFloor(bottom)){	//FF_F2
					if(_isWater(top)){	//DDf2
						tile_index = T_AAB_DDF2;
						palette = _paletteFinder(T_AAB_DDF2, bottom,middle,top);
					}
					else{
						tile_index = T_AAB_F1F1F2;
						palette = _paletteFinder(T_AAB_F1F1F2, bottom,middle,top);
					}
				}
				else if(_isFloor(top) && _isFloor(middle) && !_isFloor(bottom) && _isSameBlock(bottom,middle)){
					tile_index = T_AAB_F1F1W1;
					palette = _paletteFinder(T_AAB_F1F1W1, bottom,middle,top);

				}
				else if(!_isFloor(top) && !_isFloor(middle)){

					if(_isWater(bottom)){
						tile_index = T_AAB_WWX_W1W1D;
						palette = _paletteFinder(T_AAB_WWX_W1W1D, bottom,middle,top);
					}
					else if(_isFloor(bottom) && _isSameBlock(middle,bottom)){
						tile_index = T_AAB_WWX_W1W1F1;
						palette = _paletteFinder(T_AAB_WWX_W1W1F1, bottom,middle,top);
					}
					else if(_isFloor(bottom) && !_isSameBlock(middle,bottom)){
						tile_index = T_AAB_WWX_W1W1F2;
						palette = _paletteFinder(T_AAB_WWX_W1W1F2, bottom,middle,top);
					}
					else if(!_isFloor(bottom) && !_isSameBlock(middle,bottom)){
						tile_index = T_AAB_WWX_W1W1W2;
						palette = _paletteFinder(T_AAB_WWX_W1W1W2, bottom,middle,top);
					}

					else if(_isLeftWall(middle) && _isSameBlock(middle,bottom) && _isRightWall(bottom)){
						tile_index = T_AAB_WWX_W1W1W1B;
						palette = _paletteFinder(T_AAB_WWX_W1W1W1B, bottom,middle,top);
					}
					else if(_isRightWall(middle) && _isSameBlock(middle,bottom) && _isLeftWall(bottom)){
						tile_index = T_AAB_WWX_W1BW1BW1;
						palette = _paletteFinder(T_AAB_WWX_W1BW1BW1, bottom,middle,top);
					}

					else{
						printf("ERROR - pattern: AAB_WWX\n %d, %x,%x,%x\n",j, bottom, middle, top);
					}
				}

				else{
					printf("ERROR - pattern: AAB\n %d, %x,%x,%x\n",j, bottom, middle, top);
				}
			}
			/* ABB */
			else if(middle == bottom){
				if(_isFloor(top) && !_isFloor(middle) && !_isFloor(bottom)){	//F1W1W1
					tile_index = T_ABB_F1W1W1;
					palette = _paletteFinder(T_ABB_F1W1W1, bottom,middle,top);
				}
				else if(!_isFloor(top) && _isWater(middle) && _isWater(bottom)){
					tile_index = T_ABB_WFF_W1DD;
					palette = _paletteFinder(T_ABB_WFF_W1DD, bottom,middle,top);
				}
				else if( _isFloor(middle) && _isFloor(bottom)){	//XFF
					if(_isWater(top)){	//DF2F2
						tile_index = T_ABB_DF2F2;
						palette = _paletteFinder(T_ABB_DF2F2, bottom, middle, top);
					}
					else if(_isFloor(top) && !_isSameBlock(top,middle)){
						tile_index = T_ABB_F1F2F2;
						palette = _paletteFinder(T_ABB_F1F2F2, bottom, middle, top);
					}
					else if(_isFloor(top) && !_isFloor(middle) && _isSameBlock(top,middle)){
						tile_index = T_ABB_F1W1W1;
						palette = _paletteFinder(T_ABB_F1W1W1, bottom, middle, top);
					}
					else if(!_isFloor(top) && _isFloor(middle)){	//WFF
						if(_isWater(bottom)){
							tile_index = T_ABB_WFF_W1DD;
							palette = _paletteFinder(T_ABB_WFF_W1DD, bottom, middle, top);
						}
						else if(_isSameBlock(middle,top)){

							tile_index = T_ABB_WFF_W2F2F2;
							palette = _paletteFinder(T_ABB_WFF_W2F2F2, bottom, middle, top);
						}
						else{
							tile_index = T_ABB_WFF_W1F2F2;
							palette = _paletteFinder(T_ABB_WFF_W1F2F2, bottom, middle, top);
						}
					}
					else{
						printf("ERROR - pattern: ABB_WFF\n %d, %x,%x,%x\n",j, bottom, middle, top);
					}
				}
				else{
					printf("ERROR - pattern: ABB\n %d, %x,%x,%x\n",j, bottom, middle, top);
				}
			}
			else{
				printf("???: %d %d %d %d\n",j,bottom,middle,top);
			}
			BG_MAP_RAM(3)[j] = isFlipped | tile_index | palette<<12;
			break;

		case 3:
			//Three different colors. most _isSameBlock tests are just to catch for errors.
			if(_isWater(top) && _isFloor(middle) && _isFloor(bottom)){	//DF2F3
				tile_index = T_ABC_DF2F3;
				palette = _paletteFinder(T_ABC_DF2F3, bottom,middle,top);
			}
			else if(_isWater(top) && _isFloor(middle) && !_isFloor(bottom) && _isSameBlock(middle,bottom)){
				tile_index = T_ABC_DF2W2;
				palette = _paletteFinder(T_ABC_DF2W2, bottom,middle,top);

			}
			else if(_isFloor(top) && _isFloor(middle) && _isFloor(bottom)){	//F1F2F3
				tile_index = T_ABC_F1F2F3;
				palette = _paletteFinder(T_ABC_F1F2F3, bottom,middle,top);
			}
			else if(_isFloor(top) && _isFloor(middle) && !_isFloor(bottom) && _isSameBlock(middle,bottom)){ //F1F2W2
				tile_index = T_ABC_F1F2W2;
				palette = _paletteFinder(T_ABC_F1F2W2, bottom,middle,top);
			}
			else if(_isFloor(top) && !_isFloor(middle) && _isFloor(bottom) && _isSameBlock(top,middle)){	//F1W1F2
				tile_index = T_ABC_F1W1F2;
				palette = _paletteFinder(T_ABC_F1W1F2, bottom,middle,top);
			}
			else if(!_isFloor(top) && _isFloor(middle) && !_isFloor(bottom)){
				if(!_isSameBlock(top,bottom)){
				tile_index = T_ABC_W1F2W2;
				palette = _paletteFinder(T_ABC_W1F2W2,bottom,middle,top);
				}
				else if(_isLeftWall(top)){
					tile_index = T_ABC_W2F2W2B;
					palette = _paletteFinder(T_ABC_W2F2W2B,bottom,middle,top);
				}
				else{
					tile_index = T_ABC_W2BF2W2;
					palette = _paletteFinder(T_ABC_W2BF2W2,bottom,middle,top);
				}
			}

			else if(!_isFloor(top) && _isFloor(middle) && _isFloor(bottom)){	//WFF

				if(_isWater(middle) && _isSameBlock(top,bottom)){
					tile_index = T_ABC_WFF_W1DF1;
					palette = _paletteFinder(T_ABC_WFF_W1DF1,bottom,middle,top);
				}
				else if(_isWater(middle)){
					tile_index = T_ABC_WFF_W1DF3;
					palette = _paletteFinder(T_ABC_WFF_W1DF3, bottom,middle,top);
				}
				else if(_isSameBlock(top,middle)){
					tile_index = T_ABC_WFF_W2F2F3;
					palette = _paletteFinder(T_ABC_WFF_W2F2F3, bottom,middle,top);
				}
				else if(_isSameBlock(top,bottom)){
					tile_index = T_ABC_WFF_W3F2F3;
					palette = _paletteFinder(T_ABC_WFF_W3F2F3, bottom,middle,top);
				}
				else{
					tile_index = T_ABC_WFF_W1F2F3;
					palette = _paletteFinder(T_ABC_WFF_W1F2F3, bottom,middle,top);
				}
			}
			else{
				printf("ERROR - pattern: ABC\n %d, %x,%x,%x\n",j, bottom, middle, top);
			}
			if(palette > 10) printf("3 - %d\n",j);
			BG_MAP_RAM(3)[j] = isFlipped | tile_index | palette<<12;
			break;

		}
	}
}

/* TILES */
u8 TILE_FULL[] = {
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77,
		0x77,0x77,0x77,0x77
};


u8 TILE_ABC_F1F2F3[] = {
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x33,0x22,0x22,0x22,
		0x33,0x33,0x22,0x22,
		0x33,0x33,0x33,0x22,
		0x33,0x33,0x33,0x33
};
u8 TILE_ABC_DF2F3[] = {
		0x00,0x00,0x00,0x22,
		0x00,0x00,0x22,0x22,
		0x00,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x33,0x22,0x22,0x22,
		0x33,0x33,0x22,0x22,
		0x33,0x33,0x33,0x22,
		0x33,0x33,0x33,0x33
};
u8 TILE_ABC_F1F2W2[] = {
		0x22,0x22,0x22,0x44,
		0x22,0x22,0x44,0x44,
		0x22,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};
u8 TILE_ABC_DF2W2[] = {
		0x00,0x00,0x00,0x44,
		0x00,0x00,0x44,0x44,
		0x00,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};
u8 TILE_ABC_F1W1F2[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x22,0x55,0x55,0x55,
		0x22,0x22,0x55,0x55,
		0x22,0x22,0x22,0x55,
		0x22,0x22,0x22,0x22
};
u8 TILE_ABC_WFF_W2F2F3[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x22,0x44,0x44,0x44,
		0x22,0x22,0x44,0x44,
		0x22,0x22,0x22,0x44,
		0x22,0x22,0x22,0x22
};
u8 TILE_ABC_WFF_W3F2F3[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x44,0x22,0x22,0x22,
		0x44,0x44,0x22,0x22,
		0x44,0x44,0x44,0x22,
		0x44,0x44,0x44,0x44
};
u8 TILE_ABC_WFF_W1F2F3[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x33,0x22,0x22,0x22,
		0x33,0x33,0x22,0x22,
		0x33,0x33,0x33,0x22,
		0x33,0x33,0x33,0x33
};
//TODO Not complete? not possible to map in one tile
u8 TILE_ABC_WFF_W1DF1[] = {
		0x55,0x55,0x55,0x00,
		0x55,0x55,0x00,0x00,
		0x55,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x44,0x00,0x00,0x00,
		0x44,0x44,0x00,0x00,
		0x44,0x44,0x44,0x00,
		0x44,0x44,0x44,0x44
};
u8 TILE_ABC_WFF_W1DF3[] = {
		0x55,0x55,0x55,0x00,
		0x55,0x55,0x00,0x00,
		0x55,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x22,0x00,0x00,0x00,
		0x22,0x22,0x00,0x00,
		0x22,0x22,0x22,0x00,
		0x22,0x22,0x22,0x22
};
//TODO Catch W1F2W2 case, W2BF2W2?
u8 TILE_ABC_W1F2W2[] = {
		0x66,0x66,0x66,0x44,
		0x66,0x66,0x44,0x44,
		0x66,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};
u8 TILE_ABC_W2F2W2B[] = {
		0x11,0x11,0x11,0x44,
		0x11,0x11,0x44,0x44,
		0x11,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};
u8 TILE_ABC_W2BF2W2[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x11,0x44,0x44,0x44,
		0x11,0x11,0x44,0x44,
		0x11,0x11,0x11,0x44,
		0x11,0x11,0x11,0x11
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
		0x11,0x11,0x11,0x11
};
u8 TILE_ABA_F1W1F1[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x44,0x44
};
u8 TILE_ABA_W1F1W1[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};

u8 TILE_AAB_F1F1F2[] = {
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x22,0x11,0x11,0x11,
		0x22,0x22,0x11,0x11,
		0x22,0x22,0x22,0x11,
		0x22,0x22,0x22,0x22
};

u8 TILE_AAB_WWX_W1W1D[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x00,0x55,0x55,0x55,
		0x00,0x00,0x55,0x55,
		0x00,0x00,0x00,0x55,
		0x00,0x00,0x00,0x00
};

u8 TILE_AAB_F1F1W1[] = {
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x55,0x55
};

u8 TILE_AAB_WWX_W1W1F2[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x22,0x55,0x55,0x55,
		0x22,0x22,0x55,0x55,
		0x22,0x22,0x22,0x55,
		0x22,0x22,0x22,0x22
};
u8 TILE_AAB_WWX_W1W1F1[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x44,0x44
};
u8 TILE_AAB_WWX_W1W1W2[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x66,0x55,0x55,0x55,
		0x66,0x66,0x55,0x55,
		0x66,0x66,0x66,0x55,
		0x66,0x66,0x66,0x66
};

u8 TILE_AAB_WWX_W1BW1BW1[] = {
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x11,0x55,0x55,0x55,
		0x11,0x11,0x55,0x55,
		0x11,0x11,0x11,0x55,
		0x11,0x11,0x11,0x11
};

u8 TILE_AAB_WWX_W1W1W1B[] = {
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x55,0x11,0x11,0x11,
		0x55,0x55,0x11,0x11,
		0x55,0x55,0x55,0x11,
		0x55,0x55,0x55,0x55
};
u8 TILE_AAB_DDF2[] = {
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x11,0x00,0x00,0x00,
		0x11,0x11,0x00,0x00,
		0x11,0x11,0x11,0x00,
		0x11,0x11,0x11,0x11
};

u8 TILE_ABB_F1W1W1[] = {
		0x44,0x44,0x44,0x55,
		0x44,0x44,0x55,0x55,
		0x44,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55
};

u8 TILE_ABB_F1F2F2[] = {
		0x11,0x11,0x11,0x22,
		0x11,0x11,0x22,0x22,
		0x11,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22
};

u8 TILE_ABB_WFF_W2F2F2[] = {
		0x55,0x55,0x55,0x44,
		0x55,0x55,0x44,0x44,
		0x55,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44,
		0x44,0x44,0x44,0x44
};

u8 TILE_ABB_WFF_W1F2F2[] = {
		0x55,0x55,0x55,0x22,
		0x55,0x55,0x22,0x22,
		0x55,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22,
		0x22,0x22,0x22,0x22
};
u8 TILE_ABB_WFF_W1DD[] = {
		0x55,0x55,0x55,0x00,
		0x55,0x55,0x00,0x00,
		0x55,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00
};
u8 TILE_ABB_DF2F2[] = {
		0x00,0x00,0x00,0x11,
		0x00,0x00,0x11,0x11,
		0x00,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11,
		0x11,0x11,0x11,0x11
};

//0x0 ALL 	COLORS, 1 PER PALETTE
//0x1-0x3 	PERMUTATION OF FLOORS
//0x4		FLOORS
//0x5		RESPECTIVE WALLS FOR FLOOR 0x4
//0x6		OPPOSITE WALL FOR FLOOR	   0x4
//0x7		WATER
u16 COLOR_PALETTE[] = {
		COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,COLOR_WATER,
		COLOR_F1,	COLOR_F1, COLOR_F2, COLOR_F2, COLOR_FS, COLOR_FS, COLOR_W2A,COLOR_W1A,
		COLOR_F2, 	COLOR_FS, COLOR_F1, COLOR_FS, COLOR_F1, COLOR_F2, COLOR_FS, COLOR_FS,
		COLOR_FS, 	COLOR_F2, COLOR_FS, COLOR_F1, COLOR_F2, COLOR_F1, COLOR_F2, COLOR_F1,
		COLOR_F1,	COLOR_F1, COLOR_F2, COLOR_F2, COLOR_F2, COLOR_F1, COLOR_F2, COLOR_F1,
		COLOR_W1A, 	COLOR_W1A,COLOR_W2A,COLOR_W2A,COLOR_W2B,COLOR_W1B,COLOR_W2B,COLOR_W1B,
		COLOR_W2A,  COLOR_W2B,COLOR_W1A,COLOR_W1B,COLOR_W1A,COLOR_W2A,COLOR_W1B,COLOR_W2B,
		COLOR_WATER,COLOR_F1, COLOR_F2, COLOR_FS, COLOR_W1A,COLOR_W1B,COLOR_W2A,COLOR_W2B
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

	/* Tiles */
	dmaCopy(TILE_FULL, &BG_TILE_RAM(0)[T_FULL<<4], 32);

	dmaCopy(TILE_ABA_F1F2F1, &BG_TILE_RAM(0)[T_ABA_F1F2F1<<4], 32);
	dmaCopy(TILE_ABA_F1W1F1, &BG_TILE_RAM(0)[T_ABA_F1W1F1<<4], 32);
	dmaCopy(TILE_ABA_W1F1W1, &BG_TILE_RAM(0)[T_ABA_W1F1W1<<4], 32);

	dmaCopy(TILE_AAB_F1F1F2, &BG_TILE_RAM(0)[T_AAB_F1F1F2<<4], 32);
	dmaCopy(TILE_AAB_DDF2, &BG_TILE_RAM(0)[T_AAB_DDF2<<4], 32);
	dmaCopy(TILE_AAB_F1F1W1, &BG_TILE_RAM(0)[T_AAB_F1F1W1<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1W1F1, &BG_TILE_RAM(0)[T_AAB_WWX_W1W1F1<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1W1F2, &BG_TILE_RAM(0)[T_AAB_WWX_W1W1F2<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1W1W2, &BG_TILE_RAM(0)[T_AAB_WWX_W1W1W2<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1W1W1B, &BG_TILE_RAM(0)[T_AAB_WWX_W1W1W1B<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1BW1BW1, &BG_TILE_RAM(0)[T_AAB_WWX_W1BW1BW1<<4], 32);
	dmaCopy(TILE_AAB_WWX_W1W1D, &BG_TILE_RAM(0)[T_AAB_WWX_W1W1D<<4], 32);

	dmaCopy(TILE_ABB_F1F2F2, &BG_TILE_RAM(0)[T_ABB_F1F2F2<<4], 32);
	dmaCopy(TILE_ABB_F1W1W1, &BG_TILE_RAM(0)[T_ABB_F1W1W1<<4], 32);
	dmaCopy(TILE_ABB_WFF_W1F2F2, &BG_TILE_RAM(0)[T_ABB_WFF_W1F2F2<<4], 32);
	dmaCopy(TILE_ABB_WFF_W2F2F2, &BG_TILE_RAM(0)[T_ABB_WFF_W2F2F2<<4], 32);
	dmaCopy(TILE_ABB_WFF_W1DD, &BG_TILE_RAM(0)[T_ABB_WFF_W1DD<<4], 32);
	dmaCopy(TILE_ABB_DF2F2, &BG_TILE_RAM(0)[T_ABB_DF2F2<<4], 32);

	dmaCopy(TILE_ABC_F1F2F3, &BG_TILE_RAM(0)[T_ABC_F1F2F3<<4], 32);
	dmaCopy(TILE_ABC_F1F2W2, &BG_TILE_RAM(0)[T_ABC_F1F2W2<<4], 32);
	dmaCopy(TILE_ABC_F1W1F2, &BG_TILE_RAM(0)[T_ABC_F1W1F2<<4], 32);
	dmaCopy(TILE_ABC_W1F2W2, &BG_TILE_RAM(0)[T_ABC_W1F2W2<<4], 32);
	dmaCopy(TILE_ABC_W2BF2W2, &BG_TILE_RAM(0)[T_ABC_W2BF2W2<<4], 32);
	dmaCopy(TILE_ABC_W2F2W2B, &BG_TILE_RAM(0)[T_ABC_W2F2W2B<<4], 32);
	dmaCopy(TILE_ABC_WFF_W1F2F3, &BG_TILE_RAM(0)[T_ABC_WFF_W1F2F3<<4], 32);
	dmaCopy(TILE_ABC_WFF_W2F2F3, &BG_TILE_RAM(0)[T_ABC_WFF_W2F2F3<<4], 32);
	dmaCopy(TILE_ABC_WFF_W3F2F3, &BG_TILE_RAM(0)[T_ABC_WFF_W3F2F3<<4], 32);
	dmaCopy(TILE_ABC_WFF_W1DF3, &BG_TILE_RAM(0)[T_ABC_WFF_W1DF3<<4], 32);
	dmaCopy(TILE_ABC_WFF_W1DF1, &BG_TILE_RAM(0)[T_ABC_WFF_W1DF1<<4], 32);
	dmaCopy(TILE_ABC_DF2F3, &BG_TILE_RAM(0)[T_ABC_DF2F3<<4], 32);
	dmaCopy(TILE_ABC_DF2W2, &BG_TILE_RAM(0)[T_ABC_DF2W2<<4], 32);
}

