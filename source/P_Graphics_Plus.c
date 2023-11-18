/*
 * P_Graphics_Plus.c
 *
 *  Created on: Oct 20, 2023
 *      Author: nds
 */
extern int coords(int,int,int);
extern int sign(int);
extern int clamp(int x, int lower, int upper);
#include "P_Graphics_Plus.h"

/*
# define M_3PI_2   3*M_PI_2
void DrawAngledLine(enum BUFFER_TYPE bT, int x, int y, double angle,
		double distance, u16 color) {
	if (bT == SUB)
		return;

	//These are the current coordinates in the screen as doubles.
	double rx = x + 0.5;
	double ry = y + 0.5;

	//The amount each value will change by moving one unit forwards.
	double dx = cos(angle);
	double dy = sin(angle);
	double dtan = tan(angle);
	bool ray_facing_down = angle > M_PI;
	bool ray_facing_right = angle < M_PI_2 || angle > M_3PI_2;
	bool doing_x = abs(dx) < abs(dy);
	while(distance > 0){
		double lrx = rx, lry = ry;
		if(doing_x){
			rx = (int)(rx + ray_facing_right*2-1);
			ry -= dtan*(rx-lrx);
			DrawVerticalLine(bT, (int)lry, (int)ry, (int)lrx, color);
			distance -= abs(ry-lry);

		}
		else{
			ry = (int)(ry + ray_facing_down*2-1);
			rx -= (ry-lry)/dtan;
			DrawHorizontalLine(bT, (int)lrx, (int)rx, (int)lry, color);
			distance -= abs(rx-lrx);
		}

		doing_x = !doing_x;
		//printf("%d #%.2f, %.2f - d: %.2f\n",doing_x, rx-lrx,ry-lry,distance);
	}
}
*/

void DrawAngledLine(enum BUFFER_TYPE bT, int x, int y, float angle,
		float distance, u16 color) {
	DrawLine(bT,x,y,x+cos(angle)*distance,y+sin(angle)*distance,color);
}

/*
 *
 */
void DrawLine(enum BUFFER_TYPE bT, int x, int y, int x2, int y2,
		u16 color) {

	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferH = get_buffer_height(bT);
	int P_BufferW = get_buffer_width(bT);

	//if(x>x2){int t = x2; x2 = x; x = t;}

	//if(y>y2){int t = y2; y2 = y; y = t;}
	int dx = x2 - x;
	int dy = y2 - y;
	if(abs(dx) > abs(dy)){
		float slope = (float)dy / (float)dx;

		//clip into frame
		int nx = clamp(x, 0, P_BufferW);
		y += slope*(nx-x);
		x = nx;
		nx = clamp(x2, 0, P_BufferW);
		y2 += slope*(nx-x2);
		x2 = nx;
		if(y<0 || y2 < 0 || y > P_BufferH || y2 > P_BufferH)
			return;

		float draw_y = y;

		int draw_x;
		for(draw_x = x; draw_x*sign(dx) <= x2 * sign(dx) && sign(dx); draw_x += sign(dx)){
			P_Buffer[coords(draw_x,draw_y,P_BufferW)] = color;
			draw_y += slope*sign(dx);
		}
	}
	else{
		float slope = (float)dx/(float)dy;
		//clip into frame
		int ny = clamp(y, 0, P_BufferH);
		x += slope*(ny-y);
		y = ny;

		ny = clamp(y2, 0, P_BufferH);
		x2 += slope*(ny-y2);
		y2 = ny;
		if(x<0 || x2 < 0 || x > P_BufferW || x2 > P_BufferW)
			return;
		float draw_x = x;

		int draw_y;
		for(draw_y = y; draw_y*sign(dy) <= y2*sign(dy) && sign(dy); draw_y += sign(dy)){
			P_Buffer[coords(draw_x,draw_y,P_BufferW)] = color;
			draw_x += slope*sign(dy);
		}
	}
}


void DrawCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color){
	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferW = get_buffer_width(bT);
	float theta;
	for(theta = 0; theta < 2* M_PI; theta += M_PI_2 / (1.1*radius)){
		P_Buffer[coords(x+cos(theta)*radius,y+sin(theta)*radius,P_BufferW)] = color;
	}
}

void FillCircle(enum BUFFER_TYPE bT, int x, int y, float radius, u16 color){
	/*int r = 1;
	for(r = 1; r < radius; r++){
		DrawCircle(bT, x, y, r, false, color);
	}*/
	float theta;
	for(theta = 0; theta < 2* M_PI; theta += M_PI_2 / (1.1*radius)){
		DrawAngledLine(bT,x,y,theta,radius,color);
	}

}
