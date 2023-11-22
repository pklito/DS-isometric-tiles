#include <math.h>
#include <nds.h>
#include "G_IsoRender.h"
#include "P_Graphics.h"
#include "G_Controls.h"
#include "game.h"

int main(void)
{
	consoleDemoInit();

	Graphics_SetupMain();
	Game_InitInput();
	Game_InitScreen();
	while(1) {
		Game_HandleInput();

		swiWaitForVBlank();
	}
}
