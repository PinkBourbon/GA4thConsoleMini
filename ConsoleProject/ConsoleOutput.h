#pragma once
#include <windows.h>

#define FG_BLACK	0
#define FG_BLUE		FOREGROUND_BLUE
#define FG_GREEN	FOREGROUND_GREEN
#define FG_RED		FOREGROUND_RED
#define FG_YELLOW	FOREGROUND_RED | FOREGROUND_GREEN
#define FG_PURPLE	FOREGROUND_RED | FOREGROUND_BLUE
#define FG_AQUA		FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_WHITE	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

#define FG_LBLUE	FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define FG_LGREEN	FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define FG_LRED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define FG_LYELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define FG_LPURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define FG_LAQUA	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_LGRAY	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_PINK		FG_LPURPLE


namespace renderer
{
	struct Pixel;
	struct Mesh;
	struct RenderFace;
}


class ConsoleOUT
{
public:
	ConsoleOUT(int width = 99, int height = 54, int fontSize = 2);
	~ConsoleOUT();

	void PutDotCurrentScreen(int x, int y, int grayscale = 0);
	void PutDotBackBuffer(int x, int y, int grayscale = 0);
	void DrawLineBackBuffer(int x1, int y1, int x2, int y2, int grayScale = 0);
	void DrawLineCurrentScreen(int x1, int y1, int x2, int y2, int grayScale = 0);
	void DrawBackBuffer(renderer::Pixel const* const* drawArr, int sizeX, int sizeY);
	void DrawBackBufferWithNoLight(renderer::Pixel const* const* drawArr, int sizeX, int sizeY);
	void SwapBackBuffer();
	void ClearBackBuffer();

	void PrintDebugVertexPos(renderer::RenderFace* face, int index = 0);
private:
	void SetScreenSize(int width, int height, int i);
	void SetCurser(int x, int y, bool isBackBuffer);
private:
	int _width;
	int _height;
	HANDLE _output[2];
	HANDLE _consoleHandle;
	int _backBufferIndex;

	const char _dot[22][3] = 
	{ 
		"  ", "..", "--", "++", "**", 
		"ii", "jj", "11", "xx", "uu", 
		"ss", "44", "aa", "33", "dd",
		"PP", "99", "NN", "SS", "GG",
		"WW", "@@"
	};
	const int _grayscale = 23;

	char* _buffer;
};
