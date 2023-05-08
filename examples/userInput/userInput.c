#include <stdio.h>
#include "OGL2D.h"

int main(void){
	OGL2D_Init(); // Initialize OGL2D

	OGL2D_CreateWindow(800, 600, "OGL2D Example - User Input"); // Create window
	OGL2D_SetWindowColor(OGL2D_GRAY); // Set window color

	OGL2D_LoadShader("vertex.shader", "fragment.shader"); // Load shaders

	OGL2D_Texture wallTexture = OGL2D_LoadTexture("wall.jpg", 0); // Load texture

	// Define variables for movement
	float wallX = -0.5;
	float wallY = -0.5;

	float deltaTime;

	printf("WASD or arrow keys to move wall\n");

	while(!OGL2D_WindowShouldClose()){
		deltaTime = OGL2D_GetFrameTime();

		// Calculate wall position based on recieved input
		if(OGL2D_IsKeyPressed(OGL2D_KEY_W) || OGL2D_IsKeyPressed(OGL2D_KEY_UP))
			wallY += 5.0f * deltaTime;
		if(OGL2D_IsKeyPressed(OGL2D_KEY_S) || OGL2D_IsKeyPressed(OGL2D_KEY_DOWN))
			wallY -= 5.0f * deltaTime;

		if(OGL2D_IsKeyPressed(OGL2D_KEY_A) || OGL2D_IsKeyPressed(OGL2D_KEY_LEFT))
			wallX -= 5.0f * deltaTime;
		if(OGL2D_IsKeyPressed(OGL2D_KEY_D) || OGL2D_IsKeyPressed(OGL2D_KEY_RIGHT))
			wallX += 5.0f * deltaTime;

		OGL2D_StartRendering();

			OGL2D_DrawTexture(wallTexture, wallX, wallY, wallTexture.aspectRatio, 1); // Draw loaded texture at position

		OGL2D_StopRendering();
	}

	OGL2D_DestroyTexture(&wallTexture); // Destroy loaded texture

	OGL2D_DestroyWindow(); // Destroy OGL2D context

	return 0;
}
