#include "ConsoleOutput.h"
#include "RendererCore.h"
#include <windows.h>
#include "RendererData.h"

#define CRASH { int x = *(int *)0; }


ConsoleOUT::ConsoleOUT(int width /* = 99*/, int height /* = 54*/, int fontSize /* = 2*/)
	:_backBufferIndex(1), _width(width), _height(height)
{
	//_CONSOLE_FONT_INFOEX fontInfoEx;
	//fontInfoEx.cbSize = sizeof(fontInfoEx);
	//fontInfoEx.nFont = 0;
	//fontInfoEx.dwFontSize.X = 0;
	//fontInfoEx.dwFontSize.Y = 5;
	//fontInfoEx.FontFamily = FF_DONTCARE;
	//fontInfoEx.FontWeight = FW_NORMAL;
	//wcscpy_s(fontInfoEx.FaceName, L"Consolas");
	//SetCurrentConsoleFontEx(_consoleHandle, FALSE, &fontInfoEx);

	//printf("설정창의 폰트 사이즈가 2가 될 때까지 \n Ctrl + 마우스 휠을 이용해 줄여주세요!");
	//system("pause");

	_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	bool ret = GetCurrentConsoleFontEx(_consoleHandle, false, &fontInfo);
	fontInfo.dwFontSize.X = fontSize / 2;
	fontInfo.dwFontSize.Y = fontSize;
	ret = SetCurrentConsoleFontEx(_consoleHandle, false, &fontInfo);

	COORD size;
	size.X = width * 2;
	size.Y = height;
	SetConsoleScreenBufferSize(_consoleHandle, size);

	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = width * 2 - 1;
	rect.Bottom = height - 1;
	SetConsoleWindowInfo(_consoleHandle, TRUE, &rect);

	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;

	for (int i = 0; i < 2; ++i)
	{
		_output[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

		SetConsoleCursorInfo(_output[i], &cursorInfo);
		SetScreenSize(width * 2, height, i);

	}
	SetConsoleActiveScreenBuffer(_output[0]);

	_buffer = (char*)malloc(width * 2);
}

ConsoleOUT::~ConsoleOUT()
{
	free(_buffer);
	CloseHandle(_output[0]);
	CloseHandle(_output[1]);
}

void ConsoleOUT::SetScreenSize(int width, int height, int index)
{
	COORD screen;
	screen.X = width;
	screen.Y = height;

	bool retval = SetConsoleScreenBufferSize(_output[index], screen);
	if (!retval)
	{
		int error = GetLastError();
		//CRASH;
	}

	SMALL_RECT r;
	r.Left = r.Top = 0;
	r.Right = width - 1;
	r.Bottom = height - 1;
	SetConsoleWindowInfo(_output[index], TRUE, &r);
}

void ConsoleOUT::PutDotCurrentScreen(int x, int y, int grayscale)
{
	if ((x < 0 || x > _width - 1) || (y < 0 || y > _height - 1))
	{
		return;
	}

	DWORD dw;
	SetCurser(x, y, false);
	WriteFile(_output[(_backBufferIndex + 1) % 2], _dot[1], 3, &dw, NULL);
}

void ConsoleOUT::PutDotBackBuffer(int x, int y, int grayscale)
{
	if ((x < 0 || x > _width - 1) || (y < 0 || y > _height - 1))
	{
		return;
	}

	DWORD dw;
	SetCurser(x, y, true);
	WriteFile(_output[_backBufferIndex], _dot[1], 3, &dw, NULL);
	//printf("▒ ");
}

void ConsoleOUT::DrawLineBackBuffer(int x1, int y1, int x2, int y2, int grayScale /*= 0*/)
{
	if (x1 > x2)
	{
		return DrawLineBackBuffer(x2, y2, x1, y1, grayScale);
	}

	DWORD dw;
	double dy = (double)(y2 - y1);
	double dx = (double)(x2 - x1);
	int dirY = dy > 0 ? 1 : -1;
	dy *= dirY;
	double e = 0;

	if (dy > dx)
	{
		while (y1 * dirY < y2 * dirY)
		{
			PutDotBackBuffer(x1, y1, grayScale);
			y1 += dirY;
			e += dx;
			if (e >= dy)
			{
				e -= dy;
				++x1;
			}
		}
	}
	else
	{
		while (x1 < x2)
		{
			PutDotBackBuffer(x1, y1, grayScale);
			++x1;
			e += dy;
			if (e >= dx)
			{
				e -= dx;
				y1 += dirY;
			}
		}
	}

	PutDotBackBuffer(x1, y1, grayScale);
}

void ConsoleOUT::DrawLineCurrentScreen(int x1, int y1, int x2, int y2, int grayScale /*= 0*/)
{
	DWORD dw;
	double dx = (double)(x2 - x1);
	double dy = (double)(y2 - y1);
	int dirX = dx > 0 ? 1 : -1;
	int dirY = dy > 0 ? 1 : -1;
	dx *= dirX;
	dy *= dirY;
	double e = 0;

	if (dy > dx)
	{
		while (y1 * dirY < y2 * dirY)
		{
			PutDotCurrentScreen(x1, y1, grayScale);
			y1 += dirY;
			e += dx;
			if (e >= dy)
			{
				e -= dy;
				x1 += dirX;
			}
		}
	}
	else
	{
		while (x1 * dirX < x2 * dirX)
		{
			PutDotCurrentScreen(x1, y1, grayScale);
			x1 += dirX;
			e += dy;
			if (e >= dx)
			{
				e -= dx;
				y1 += dirY;
			}
		}
	}

	PutDotCurrentScreen(x1, y1, grayScale);
}

void ConsoleOUT::DrawBackBuffer(renderer::Pixel const* const* drawArr, int sizeX, int sizeY)
{
	DWORD dw;
	int bufferSize = sizeX * 2;
	int index = 0;

	for (int y = 0; y < sizeY; ++y)
	{
		int index = 0;
		for (int x = 0; x < sizeX; ++x)
		{

			if ((drawArr[y] + x)->_z == -2)
			{
				_buffer[index++] = ' ';
				_buffer[index++] = ' ';
			}
			else
			{
				_buffer[index++] = _dot[10][0];
				_buffer[index++] = _dot[10][1];
			}
			/*
			int lightIntensity = ((drawArr[y] + x)->_lightIntensity) * 21;
			lightIntensity = lightIntensity < 0 ? 0 : lightIntensity;
			lightIntensity = lightIntensity > 21 ? 21 : lightIntensity;
			_buffer[index++] = _dot[lightIntensity][0];
			_buffer[index++] = _dot[lightIntensity][1];*/
		}
		SetCurser(0, y, true);
		WriteFile(_output[_backBufferIndex], _buffer, bufferSize, &dw, NULL);
	}
}

void ConsoleOUT::DrawBackBufferWithNoLight(renderer::Pixel const* const* drawArr, int sizeX, int sizeY)
{
	DWORD dw;
	int bufferSize = sizeX * 2;
	int index = 0;

	for (int y = 0; y < sizeY; ++y)
	{
		int index = 0;
		for (int x = 0; x < sizeX; ++x)
		{

			if ((drawArr[y] + x)->_z == -2)
			{
				_buffer[index++] = ' ';
				_buffer[index++] = ' ';
			}
			else
			{
				_buffer[index++] = _dot[12][0];
				_buffer[index++] = _dot[12][1];
			}
			/*
			int lightIntensity = ((drawArr[y] + x)->_lightIntensity) * 21;
			lightIntensity = lightIntensity < 0 ? 0 : lightIntensity;
			lightIntensity = lightIntensity > 21 ? 21 : lightIntensity;
			_buffer[index++] = _dot[lightIntensity][0];
			_buffer[index++] = _dot[lightIntensity][1];*/
		}
		SetCurser(0, y, true);
		WriteFile(_output[_backBufferIndex], _buffer, bufferSize, &dw, NULL);
	}
}

void ConsoleOUT::SwapBackBuffer()
{
	SetConsoleActiveScreenBuffer(_output[_backBufferIndex]);
	_backBufferIndex = (_backBufferIndex + 1) % 2;
	ClearBackBuffer();
}

void ConsoleOUT::ClearBackBuffer()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(_output[_backBufferIndex], '\0', _width * _height * 2, pos, &dw);
}

void ConsoleOUT::PrintDebugVertexPos(renderer::RenderFace* face, int index /*= 0*/)
{
	for (int i = 0; i < 3; ++i)
	{
		face[index]._vertex[i]._pos._x;
		face[index]._vertex[i]._pos._y;
		face[index]._vertex[i]._pos._z;
	}

	//WriteFile(_output[_backBufferIndex], )
	//printf("%d :: x : % .3f, y : % .3f, z : % .3f\n",
	//	i,
	//	meshArr[index]->_vertexBuffer[i]._pos._x,
	//	meshArr[index]->_vertexBuffer[i]._pos._y,
	//	meshArr[index]->_vertexBuffer[i]._pos._z
	///*meshArr[index]->_vertexBuffer[i]._pos._w*/);
}

void ConsoleOUT::SetCurser(int x, int y, bool isBackBuffer)
{
	COORD pos = { x * 2, y };
	int index = _backBufferIndex;
	if (!isBackBuffer)
	{
		index = (index + 1) % 2;
	}
	SetConsoleCursorPosition(_output[index], pos);
}

#undef CRASH