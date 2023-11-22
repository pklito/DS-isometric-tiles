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
#define WORLD_DIM_X 8
#define WORLD_DIM_Y 8
#define WORLD_DIM_Z 3

/* VISUALS */
//where zero zero starts on screen
#define TILES_ORIGIN 109
#define TILES_SHAPE BG_32x32
#define TILES_SHAPE_WIDTH 32
#define TILES_SHAPE_HEIGHT 32


/* COLORS */
#define COLOR_F1 RGB15(5,31,5)
#define COLOR_F2 RGB15(15,5,5)
#define COLOR_FS RGB15(20,20,5)
#define COLOR_W1A RGB15(12,16,12)
#define COLOR_W1B RGB15(8,14,8)
#define COLOR_W2A RGB15(14,8,8)
#define COLOR_W2B RGB15(10,6,6)
#define COLOR_WATER RGB15(5,5,31)
/* MATH
#define COLOR_F1 1
#define COLOR_F2 2
#define COLOR_FS 3
#define COLOR_W1A 4
#define COLOR_W1B 5
#define COLOR_W2A 6
#define COLOR_W2B 7
#define COLOR_WATER 8/*/
