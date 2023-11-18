#pragma once

#include <stdio.h>
#include <nds.h>
#include "P_Util.h"
#include "Constants.h"

#define ROTOSCALE
#define TILES

enum BUFFER_TYPE
{
    MAIN,
    SUB,
};

//members
extern u16* P_Graphics_mainBuffer;
extern u16* P_Graphics_subBuffer;
extern int P_Graphics_mainW;
extern int P_Graphics_mainH;
extern int P_Graphics_subW;
extern int P_Graphics_subH;

/*
 * get the current buffer
 */
inline u16* get_buffer_pointer(enum BUFFER_TYPE bT);
inline int get_buffer_width(enum BUFFER_TYPE bT);
inline int get_buffer_height(enum BUFFER_TYPE bT);
/*
	This function does not receive any input parameter. It initializes the main
	 graphical engine to be used with the Framebuffer Mode 0 (FB0).
	It also enables the corresponding VRAM bank to be used.
*/
void Graphics_SetupMain();
void Graphics_SetupSub();


/*
 	 Function that assigns the buffer (already implemented) and its size
 */
void Graphics_AssignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h);


/*
 	 This function fills all the screen of the given engine (MAIN or SUB)
 	 with the input color.
 */
void FillScreen(enum BUFFER_TYPE t, u16 color);


/*
	This function fills a rectangle specified by the coordinates (top,left)
	and (bottom,right) with the given color.

The input parameters are the following
-	BUFFER_TYPE bT: This parameter specifies the buffer where we want to fill
 	the rectangle. Remember that there are two graphical engines (MAIN and SUB)
 	for both screens of the NDS. In this practical work we will only use the
 	MAIN engine since Framebuffer Mode is only available in this engine.
-	int top, bottom, left, right: These parameters specify the coordinates of
	the rectangle that we want to fill. It is recommended to check that these
	coordinates are correct (under the buffer boundaries) before starting
	drawing the rectangle.
-	u16 color: This is the color that we will use to fill the rectangle.
	Remember that colors are represented with 16bit words in the Nintendo DS.
*/
void FillRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color);


/*
	This function draws an horizontal line of 1 pixel width between the
	coordinates (x,y) and (x2,y) with the given color.

The input parameters are the following
-	BUFFER_TYPE bT: This parameter is the same to the one received in the
	previous function.
-	int x, x2, y: These parameters specify the coordinates of the line that we
	want to draw. It is recommended to check that this coordinates are correct
	(under the buffer boundaries) before starting drawing the line.
-	u16 color: This parameter is the color like explained in the previous
	function.
*/
void DrawHorizontalLine(enum BUFFER_TYPE bT, int x, int x2, int y, u16 color);


/*
	This function draws a vertical line of 1 pixel width between the coordinates
	(x,y) and (x,y2) with the given color.

The input parameters are the following
-	BUFFER_TYPE bT: This parameter is the same to the one received in the
	previous functions.
-	int y, y2, x: These parameters specify the coordinates of the line that we
	want to draw. It is recommended to check that this coordinates are correct
	(under the buffer boundaries) before starting drawing the line.
-	u16 color: This parameter is the color like explained in the previous
	functions.
*/
void DrawVerticalLine(enum BUFFER_TYPE bT, int y, int y2, int x, u16 color);


/*
	This function draws a rectangle specified by the coordinates (top,left)
	and (bottom,right) with the given color. Note that this function is
	different to the one that fills the rectangle. This function only draws
	the rectangle (the borders). It is recommended to re-use the already
	implemented functions to draw lines.

The input parameters are the following
-	BUFFER_TYPE bT: This parameter is the same to the one received in the
	previous functions.
-	int top, bottom, left, right: These parameters specify the coordinates of
	the rectangle that we want to draw. It is recommended to check that this
	coordinates are correct (under the buffer boundaries) before starting
	drawing the rectangle.
-	u16 color: This is the color that we will use to draw the borders of the
	rectangle.
*/

void DrawRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color);

void Graphics_SwapBuffers(enum BUFFER_TYPE bT);
