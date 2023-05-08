#include "OGL2D.h"

int main(void){
	OGL2D_Init(); // Initialize OGL2D

	OGL2D_CreateWindow(800, 600, "OGL2D Example - Creating Window"); // Create the window with width of 800, height of 600 and title
	OGL2D_SetWindowColor(OGL2D_GRAY); // Set window color to gray

	while(!OGL2D_WindowShouldClose()){
		// Without these, window will not appear
		OGL2D_StartRendering();

		OGL2D_StopRendering();
	}

	OGL2D_DestroyWindow(); // Destroy OGL2D context

	return 0;
}
