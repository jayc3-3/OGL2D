#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#define GLT_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "glText.h"
#include "stb_image.h"

#include "OGL2D.h"

GLFWwindow* OGL2D_Window;

mat4 camProj;
mat4 camView;
mat4 camModel;
mat4 camMVP;

unsigned int matrixID;

unsigned int vertexArray;
unsigned int shaderProgram;

int textureAmount = 0;

bool OGL2D_DebugMode = false;

int frameCounter = 0;
double currentTime = 0;
double lastTime = 0;

int fps = 0;

float deltaTime = 0;
double dtCurrent = 0;
double dtLast = 0;

bool useVsync = false;

const unsigned int rectangleIndices[] = {
	0, 1, 3,
	1, 2, 3
};

const float rectangleTextureData[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

unsigned int rectangleElementBuffer;
unsigned int rectangleTextureBuffer;

void OGL2D_WindowResizeEvent(GLFWwindow* window, int newWidth, int newHeight);

void OGL2D_SetWindowHint(int windowHint){
	switch(windowHint){
		case OGL2D_USE_MSAA:
			glfwWindowHint(GLFW_SAMPLES, 4);

			if(OGL2D_DebugMode)
				printf("OGL2D_INFO: Enabling 4x MSAA\n");

			break;

		case OGL2D_WINDOW_RESIZABLE:
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

			if(OGL2D_DebugMode)
				printf("OGL2D_INFO: Enabling resizable window\n");

			break;

		case OGL2D_USE_VSYNC:
			useVsync = true;

			if(OGL2D_DebugMode)
				printf("OGL2D_INFO: Enabling vsync\n");

		default:
			break;
	}
	
	return;
}

void OGL2D_SetWindowColor(OGL2D_Color color){
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);

	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Set window color to %i:%i:%i\n", color.r, color.g, color.b);

	return;
}

void OGL2D_Init(void){
	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Initializing OGL2D\n");

	if(!glfwInit()){
		printf("OGL2D_ERROR: Unable to initialize GLFW\n");
		return;
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	return;
}

void OGL2D_CreateWindow(int windowWidth, int windowHeight, const char* windowTitle){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	OGL2D_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	if(OGL2D_Window == NULL){
		printf("OGL2D_ERROR: Unable to create GLFW window\n");
		return;
	}

	glfwMakeContextCurrent(OGL2D_Window);
	
	glfwSetFramebufferSizeCallback(OGL2D_Window, OGL2D_WindowResizeEvent);

	glewExperimental = true;
	if(glewInit() != GLEW_OK){
		printf("OGL2D_ERROR: Unable to initialize GLEW\n");
		return;
	}

	glfwSwapInterval(useVsync);

	stbi_set_flip_vertically_on_load(true);

	glm_perspective(glm_rad(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f, camProj);

	glm_lookat(
		(vec3){0, 0, 4},
		(vec3){0, 0, 0},
		(vec3){0, 1, 0},
		camView
	);

	glm_mat4_identity(camModel);

	glm_mat4_mul(camProj, camView, camMVP);
	glm_mat4_mul(camMVP, camModel, camMVP);

	currentTime = glfwGetTime();
	lastTime = glfwGetTime();

	dtCurrent = glfwGetTime();
	dtLast = glfwGetTime();

	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &rectangleElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &rectangleTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleTextureData), rectangleTextureData, GL_STATIC_DRAW);

	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully initialized OpenGL 3.3 and created window\n");

	return;
}

void OGL2D_DestroyWindow(void){
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &rectangleTextureBuffer);

	glfwDestroyWindow(OGL2D_Window);
	glfwTerminate();

	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully closed OpenGL context and destroyed window\n");

	return;
}

int OGL2D_WindowShouldClose(void) {return glfwWindowShouldClose(OGL2D_Window);}
void OGL2D_CloseWindow(void) {glfwSetWindowShouldClose(OGL2D_Window, GLFW_TRUE); return;};

void OGL2D_WindowResizeEvent(GLFWwindow* window, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);

	glm_perspective(glm_rad(45.0f), (float)newWidth / (float)newHeight, 0.1f, 100.0f, camProj);

	glm_lookat(
		(vec3){0, 0, 4},
		(vec3){0, 0, 0},
		(vec3){0, 1, 0},
		camView
	);

	glm_mat4_identity(camModel);

	glm_mat4_mul(camProj, camView, camMVP);
	glm_mat4_mul(camMVP, camModel, camMVP);

	if(shaderProgram != 0)
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &camMVP[0][0]);

	return;
}

void OGL2D_StartRendering(void){
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	return;
}

void OGL2D_StopRendering(void){
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glfwSwapBuffers(OGL2D_Window);
	glfwPollEvents();

	currentTime = glfwGetTime();
	frameCounter++;

	if(currentTime - lastTime >= 1.0){
		fps = frameCounter;
		frameCounter = 0;
		lastTime = currentTime;
	}

	dtLast = dtCurrent;
	dtCurrent = glfwGetTime();

	deltaTime = (float)(dtCurrent - dtLast);

	return;
}

int OGL2D_GetFPS(void) {return fps;}
float OGL2D_GetFrameTime(void) {return deltaTime;}

int OGL2D_GetFileSize(const char* filePath){
	FILE* file = fopen(filePath, "r");

	fseek(file, 0L, SEEK_END);
	int result = ftell(file);

	fclose(file);

	return result;
}

void OGL2D_LoadShader(const char* vertexPath, const char* fragmentPath){
	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Attempting to load shaders '%s' and '%s'\n", vertexPath, fragmentPath);

	FILE* vertexFile = fopen(vertexPath, "r");
	FILE* fragmentFile = fopen(fragmentPath, "r");

	if(vertexFile == NULL){
		printf("OGL2D_ERROR: Unable to open vertex shader file from '%s'\n", vertexPath);
		return;
	}
	else if(fragmentFile == NULL){
		printf("OGL2D_ERROR: Unable to open fragment shader file from '%s'\n", fragmentPath);
		return;
	}
	else if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully loaded vertex shader and fragment shader files\n");

	int vertexSize = OGL2D_GetFileSize(vertexPath);
	int fragmentSize = OGL2D_GetFileSize(fragmentPath);

	char vertexData[vertexSize];
	char fragmentData[fragmentSize];

	{
		int i = 0;

		while(!feof(vertexFile))
		{
			vertexData[i] = fgetc(vertexFile);

			i++;
		}

		vertexData[i-1] = '\0';
	}
	{
		int i = 0;

		while(!feof(fragmentFile))
		{
			fragmentData[i] = fgetc(fragmentFile);

			i++;
		}

		fragmentData[i-1] = '\0';
	}

	fclose(vertexFile);
	fclose(fragmentFile);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexPointer = vertexData;
	const char* fragmentPointer = fragmentData;

	glShaderSource(vertexShader, 1, &vertexPointer, NULL);
	glShaderSource(fragmentShader, 1, &fragmentPointer, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	int shaderSuccess;
	char shaderLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
	if(!shaderSuccess){
		glGetShaderInfoLog(vertexShader, 512, NULL, shaderLog);
		printf("OGL2D_ERROR: Error while compiling vertex shader: %s\n", shaderLog);
		return;
	}
	else if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully compiled vertex shader\n");

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
	if(!shaderSuccess){
		glGetShaderInfoLog(fragmentShader, 512, NULL, shaderLog);
		printf("OGL2D_ERROR: Error while compiling fragment shader: %s\n", shaderLog);
		return;
	}
	else if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully compiled fragment shader\n");

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
	if(!shaderSuccess){
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderLog);
		printf("OGL2D_ERROR: Error while linking shader program: %s\n", shaderLog);
		return;
	}
	else if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Successfully linked shader program\n");

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	matrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &camMVP[0][0]);

	return;
}

OGL2D_Texture OGL2D_LoadTexture(const char* filePath, bool transparent){
	if(OGL2D_DebugMode)
		printf("OGL2D_INFO: Attempting to load texture ID '%i' from '%s'\n", textureAmount, filePath);

	OGL2D_Texture newTexture;
	glGenTextures(1, &newTexture.texture);
	glBindTexture(GL_TEXTURE_2D, newTexture.texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int imgW, imgH, imgC;
	unsigned char* imgD = stbi_load(filePath, &imgW, &imgH, &imgC, 0);

	if(imgD){
		if(transparent)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgD);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgW, imgH, 0, GL_RGB, GL_UNSIGNED_BYTE, imgD);

		glGenerateMipmap(GL_TEXTURE_2D);

		if(OGL2D_DebugMode)
			printf("OGL2D_INFO: Successfully loaded data from image\n");
	}
	else{
		printf("OGL2D_ERROR: Unable to load texture from '%s'\n", filePath);
		OGL2D_Texture emptyTexture = {0};
		return emptyTexture;
	}

	newTexture.width = imgW;
	newTexture.height = imgH;
	newTexture.aspectRatio = (float)imgW / (float)imgH;
	newTexture.id = textureAmount;

	textureAmount++;

	stbi_image_free(imgD);

	return newTexture;
}

void OGL2D_DestroyTexture(OGL2D_Texture* texture) {
	if(OGL2D_DebugMode)
	       	printf("OGL2D_INFO: Destroying texture ID '%i'\n", texture->id);
	
	glDeleteTextures(1, &texture->texture); 
	return;
}



void OGL2D_DrawTexture(OGL2D_Texture texture, float x, float y, float width, float height){
	const float rectangleVertices[] = {
		x+width, y+height, 0,
		x+width, y, 0,
		x, y, 0,
		x, y+height, 0
	};

	unsigned int rectangleVertexBuffer;
	glGenBuffers(1, &rectangleVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, rectangleVertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ARRAY_BUFFER, rectangleTextureBuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindTexture(GL_TEXTURE_2D, texture.texture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleElementBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &rectangleVertexBuffer);

	return;
}

bool OGL2D_IsKeyPressed(int key){
	bool isPressed = glfwGetKey(OGL2D_Window, key);

	return isPressed;
}
