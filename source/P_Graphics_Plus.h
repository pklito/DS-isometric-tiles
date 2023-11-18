#pragma once

#include <stdio.h>
#include <nds.h>
#include "P_Graphics.h"
#include <math.h>

/*
 *
 */
void DrawAngledLine(enum BUFFER_TYPE bT, int x, int y, float angle,float distance, u16 color);

/*
 *
 */
void DrawLine(enum BUFFER_TYPE bT, int x, int y, int x2,int y2, u16 color);


void DrawCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color);

void FillCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color);

inline u16 Gradient(u16 color1, u16 color2, byte scale);
