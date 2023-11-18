#include "P_Graphics.h"

u16* P_Graphics_mainBuffer;
u16* P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;

bool main_graphics_frame = true;
bool sub_graphics_frame = true;
void Graphics_SetupMain()
{
#ifdef FB0
	//Graphics (Part 1)
	//Set the mode to FB0
	REG_DISPCNT = MODE_FB0;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_LCD;


	Graphics_AssignBuffer(MAIN, (u16*)VRAM_A,256,192);

#endif

#ifdef ROTOSCALE
	//BG0 will be a tilemap for the sky and floor, using VRAM B
	//BG2 will be a a rotoscale for the walls, using VRAM A
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE;

	//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	BGCTRL[2] = BG_BMP_BASE(1) | BG_BMP16_256x256;

	//P_Graphics_assignBuffer(MAIN, (u16*)BG_GFX,256,192);
	REG_BG2PA = 256;
	REG_BG2PC = 0;
	REG_BG2PB = 0;
	REG_BG2PD = 256;

	Graphics_AssignBuffer(MAIN,BG_BMP_RAM(1),256,192);
#endif
#ifdef TILES
	/* Tilemap */
	BGCTRL[0] = BG_MAP_BASE(1) | BG_32x32 | BG_COLOR_16 | BG_TILE_BASE(0) | BG_PRIORITY_1;//Set the coresponding VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	int i;
	for(i=0;i<32*32;i++){
		BG_MAP_RAM(1)[i] = 0 | (i>=32*12 ? BIT(12) : 0);
	}
#endif
}

//get the current buffer to write to. (in the case of double buffering, this is NOT `bgGetGfxPtr(id)`
inline u16* get_buffer_pointer(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainBuffer : P_Graphics_subBuffer;}
inline int get_buffer_width(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainW : P_Graphics_subW;}
inline int get_buffer_height(enum BUFFER_TYPE bT){return (bT==MAIN) ? P_Graphics_mainH : P_Graphics_subH;}

void Graphics_AssignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h)
{

    switch(bT)
    {
        case MAIN: P_Graphics_mainBuffer = buffer;
            P_Graphics_mainW = w;
            P_Graphics_mainH = h;
            break;
        case SUB: P_Graphics_subBuffer = buffer;
            P_Graphics_subW = w;
            P_Graphics_subH = h;
            break;
    }
}


void FillScreen(enum BUFFER_TYPE t, u16 color)
{
	int i;
	u16* P_Buffer = get_buffer_pointer(t);
	int P_BufferH = get_buffer_height(t);
	int P_BufferW = get_buffer_width(t);

	//Fill the whole screen (256*192) with the given color
	for(i = 0; i<P_BufferH*P_BufferW; i++){
		P_Buffer[i] = color;
	}

}

void FillRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	u16* P_Buffer = get_buffer_pointer(bT);
	int P_BufferH = get_buffer_height(bT);
	int P_BufferW = get_buffer_width(bT);

	top = clamp(top, 0, P_BufferH);
	bottom = clamp(bottom, -1, P_BufferH-1);
	left = clamp(left, 0, P_BufferW);
	right = clamp(right, -1, P_BufferW-1);

	if(right < left || bottom < top)
		return;

	short i, j;

	for(j = top; j <= bottom; j++){

		for(i = left; i < right; i+=2){
			P_Buffer[coords(i,j,P_BufferW)] = color;
		}
	}
}

void DrawHorizontalLine(enum BUFFER_TYPE bT, int x, int x2, int y, u16 color){
	if(x2 >= x)
		FillRectangle(bT,y,y,x,x2,color);
	else
		FillRectangle(bT,y,y,x2,x,color);
}
void DrawVerticalLine(enum BUFFER_TYPE bT, int y, int y2, int x, u16 color)
{
	if(y2 >= y)
		FillRectangle(bT,y,y2,x,x,color);
	else
		FillRectangle(bT,y2,y,x,x,color);
}

void DrawRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{
	DrawVerticalLine(bT,top,bottom,left,color);
	DrawVerticalLine(bT,top,bottom,right,color);
	DrawHorizontalLine(bT,left,right,top,color);
	DrawHorizontalLine(bT,left,right,bottom,color);

}

void Graphics_SwapBuffers(enum BUFFER_TYPE bT){

#ifdef FB0

	switch(bT){
	case MAIN:
		if(main_graphics_frame) Graphics_AssignBuffer(MAIN,(u16*)VRAM_B,256,192);
		else Graphics_AssignBuffer(MAIN,(u16*)VRAM_A,256,192);

		//Set the coresponding VRAM bank
		if(main_graphics_frame) {REG_DISPCNT = MODE_FB0;}
		else {REG_DISPCNT = MODE_FB1;}
		main_graphics_frame = !main_graphics_frame;
		break;
	case SUB:
		break;
	}
#endif


#ifdef ROTOSCALE
	switch(bT){
	case MAIN:
		if(main_graphics_frame) Graphics_AssignBuffer(MAIN,BG_BMP_RAM(4),256,192);
		else Graphics_AssignBuffer(MAIN,BG_BMP_RAM(1),256,192);

		if(main_graphics_frame) BGCTRL[2] = BG_BMP_BASE(1) | BG_BMP16_256x256;
		else BGCTRL[2] = BG_BMP_BASE(4) | BG_BMP16_256x256;
		main_graphics_frame = !main_graphics_frame;
		break;
	case SUB:
		break;
	}
#endif
}
