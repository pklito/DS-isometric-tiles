/*
 * G_Constants.h
 *
 *  Created on: Nov 13, 2023
 *      Author: nds
 */
#pragma once

#define U16_MAX 65535
#define U8_MAX 255

/* WORLD */
#define WORLD_DIM_X 4
#define WORLD_DIM_Y 4
#define WORLD_DIM_Z 1

/* VISUALS */
//where zero zero starts on screen
#define TILES_ORIGIN 14
#define TILES_SHAPE BG_32x32
#define TILES_SHAPE_WIDTH 32
#define TILES_SHAPE_HEIGHT 32


/* COLORS */
#define COLOR_FLOOR_1 RGB15(5,31,5)
#define COLOR_FLOOR_2 RGB15(15,5,5)
#define COLOR_FLOOR_S RGB15(20,20,5)
#define COLOR_WALL_1A RGB15(12,16,12)
#define COLOR_WALL_1B RGB15(8,14,8)
#define COLOR_WALL_2B RGB15(10,6,6)
#define COLOR_WALL_2A RGB15(14,8,8)
#define COLOR_WATER RGB15(5,5,31)
/* MATH */
