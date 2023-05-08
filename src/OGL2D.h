#ifndef OGL2D
#define OGL2D

typedef enum {
	OGL2D_USE_MSAA,
	OGL2D_USE_VSYNC,
	OGL2D_WINDOW_RESIZABLE
} OGL2D_WindowHints;

typedef struct {
	int r;
	int g;
	int b;
} OGL2D_Color;

typedef struct {
	unsigned int texture;
	int width;
	int height;
	float aspectRatio;
	int id;
} OGL2D_Texture;

typedef enum {
	OGL2D_KEY_0 = 48,
	OGL2D_KEY_1 = 49,
	OGL2D_KEY_2 = 50,
	OGL2D_KEY_3 = 51,
	OGL2D_KEY_4 = 52,
	OGL2D_KEY_5 = 53,
	OGL2D_KEY_6 = 54,
	OGL2D_KEY_7 = 55,
	OGL2D_KEY_8 = 56,
	OGL2D_KEY_9 = 57,

	OGL2D_KEY_A = 65,
	OGL2D_KEY_B = 66,
	OGL2D_KEY_C = 67,
	OGL2D_KEY_D = 68,
	OGL2D_KEY_E = 69,
	OGL2D_KEY_F = 70,
	OGL2D_KEY_G = 71,
	OGL2D_KEY_H = 72,
	OGL2D_KEY_I = 73,
	OGL2D_KEY_J = 74,
	OGL2D_KEY_K = 75,
	OGL2D_KEY_L = 76,
	OGL2D_KEY_M = 77,
	OGL2D_KEY_N = 78,
	OGL2D_KEY_O = 79,
	OGL2D_KEY_P = 80,
	OGL2D_KEY_Q = 81,
	OGL2D_KEY_R = 82,
	OGL2D_KEY_S = 83,
	OGL2D_KEY_T = 84,
	OGL2D_KEY_U = 85,
	OGL2D_KEY_V = 86,
	OGL2D_KEY_W = 87,
	OGL2D_KEY_X = 88,
	OGL2D_KEY_Y = 89,
	OGL2D_KEY_Z = 90,

	OGL2D_KEY_ESCAPE = 256,
	OGL2D_KEY_ENTER = 257,
	OGL2D_KEY_TAB = 258,
	OGL2D_KEY_CAPSLOCK = 280,

	OGL2D_KEY_LSHIFT = 340,
	OGL2D_KEY_RSHIFT = 344,
	OGL2D_KEY_LCTRL = 341,
	OGL2D_KEY_RCTRL = 345,
	OGL2D_KEY_LALT = 342,
	OGL2D_KEY_RALT = 346,

	OGL2D_KEY_RIGHT = 262,
	OGL2D_KEY_LEFT = 263,
	OGL2D_KEY_DOWN = 264,
	OGL2D_KEY_UP = 265,

	OGL2D_KEY_F1 = 290,
	OGL2D_KEY_F2 = 291,
	OGL2D_KEY_F3 = 292,
	OGL2D_KEY_F4 = 293,
	OGL2D_KEY_F5 = 294,
	OGL2D_KEY_F6 = 295,
	OGL2D_KEY_F7 = 296,
	OGL2D_KEY_F8 = 297,
	OGL2D_KEY_F9 = 298,
	OGL2D_KEY_F10 = 299,
	OGL2D_KEY_F11 = 300,
	OGL2D_KEY_F12 = 301
} OGL2D_Keys;

extern int OGL2D_DebugMode;

#define OGL2D_WHITE (OGL2D_Color){255, 255, 255}
#define OGL2D_BLACK (OGL2D_Color){0, 0, 0}
#define OGL2D_GRAY (OGL2D_Color){160, 160, 160}

void OGL2D_Init(void);

void OGL2D_SetWindowHint(int windowHint);
void OGL2D_SetWindowColor(OGL2D_Color color);
void OGL2D_CreateWindow(int windowWidth, int windowHeight, const char* windowTitle);
void OGL2D_DestroyWindow(void);

int OGL2D_WindowShouldClose(void);
void OGL2D_CloseWindow(void);

int OGL2D_GetFPS(void);
float OGL2D_GetFrameTime(void);

void OGL2D_LoadShader(const char* vertexPath, const char* fragmentPath);

OGL2D_Texture OGL2D_LoadTexture(const char* filePath, int transparent);
void OGL2D_DestroyTexture(OGL2D_Texture* texture);

int OGL2D_IsKeyPressed(int key);

void OGL2D_StartRendering(void);
void OGL2D_StopRendering(void);

void OGL2D_DrawTexture(OGL2D_Texture texture, float x, float y, float width, float height);

#endif
