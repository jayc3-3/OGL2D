#include "OGL2D.h"

int main(void){
	OGL2D_Init(); // Initialize OGL2D

	OGL2D_CreateWindow(800, 600, "OGL2D Example - Drawing Texture"); // Create window
	OGL2D_SetWindowColor(OGL2D_GRAY); // Set window color

	OGL2D_LoadShader("vertex.shader", "fragment.shader"); // Load shaders

	OGL2D_Texture wallTexture = OGL2D_LoadTexture("wall.jpg", 0); // Load texture

	while(!OGL2D_WindowShouldClose()){
		OGL2D_StartRendering();

			OGL2D_DrawTexture(wallTexture, -0.5, -0.5, wallTexture.aspectRatio, 1); // Draw loaded texture

		OGL2D_StopRendering();
	}

	OGL2D_DestroyTexture(&wallTexture); // Destroy loaded texture

	OGL2D_DestroyWindow(); // Destroy OGL2D context

	return 0;
}
