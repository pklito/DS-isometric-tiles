#include <math.h>
#include <nds.h>
#include "G_Isometric.h"
#include "P_Graphics.h"

s8 WORLD_MAP[] = {
		1,1,1,1,
		1,1,1,1,
		1,1,0,0,
		1,1,0,0
};

int main(void)
{
	consoleDemoInit();

	Graphics_SetupMain();

	//printf("%d\n", convertWorldToTile(0,0,0));
	RenderTiles(WORLD_MAP);
	while(1) {
		//Graphics_SwapBuffers(MAIN);

		swiWaitForVBlank();
	}
}
