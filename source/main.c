#include <math.h>
#include <nds.h>
#include "G_Isometric.h"

s8 WORLD_MAP[] = {
		1,1,1,1,
		1,1,1,1,
		1,1,0,0,
		1,1,0,0
};

int main(void)
{
	consoleDemoInit();
	//printf("%d\n", convertWorldToTile(0,0,0));
	printf("%d\n", convertWorldToTile(3,0,1));
	while(1) {
		//swap_buffers(MAIN);

		swiWaitForVBlank();
	}
}
