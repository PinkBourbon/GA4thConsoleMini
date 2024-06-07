#pragma once

#include <Windows.h>

static HANDLE houtput = NULL;
static int current_foreground = 0;
static int current_background = 0;

//
// kolory znaku - ciemne
//
#define FG_BLACK	0
#define FG_BLUE		FOREGROUND_BLUE
#define FG_GREEN	FOREGROUND_GREEN
#define FG_RED		FOREGROUND_RED
#define FG_YELLOW	FOREGROUND_RED | FOREGROUND_GREEN
#define FG_PURPLE	FOREGROUND_RED | FOREGROUND_BLUE
#define FG_AQUA		FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_WHITE	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//
// kolory znaku - jasne
//
#define FG_LBLUE	FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define FG_LGREEN	FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define FG_LRED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define FG_LYELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define FG_LPURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define FG_LAQUA	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_LGRAY	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FG_PINK		FG_LPURPLE

//
// kolory t³a - ciemne
//
#define BG_BLACK	0
#define BG_BLUE		BACKGROUND_BLUE
#define BG_GREEN	BACKGROUND_GREEN
#define BG_RED		BACKGROUND_RED
#define BG_YELLOW	BACKGROUND_RED | BACKGROUND_GREEN
#define BG_PURPLE	BACKGROUND_RED | BACKGROUND_BLUE
#define BG_AQUA		BACKGROUND_GREEN | BACKGROUND_BLUE
#define BG_WHITE	BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE

//
// kolory t³a - jasne
//
#define BG_LBLUE	BACKGROUND_INTENSITY | BACKGROUND_BLUE
#define BG_LGREEN	BACKGROUND_INTENSITY | BACKGROUND_GREEN
#define BG_LRED		BACKGROUND_INTENSITY | BACKGROUND_RED
#define BG_LYELLOW	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN
#define BG_LPURPLE	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE
#define BG_LAQUA	BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE
#define BG_LGRAY	BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE


//
// kolory znaku - ciemne
//
#define BLACK			0
#define BLUE			FOREGROUND_BLUE
#define GREEN			FOREGROUND_GREEN
#define RED				FOREGROUND_RED
#define YELLOW			FOREGROUND_RED | FOREGROUND_GREEN
#define PURPLE			FOREGROUND_RED | FOREGROUND_BLUE
#define AQUA			FOREGROUND_GREEN | FOREGROUND_BLUE
#define WHITE			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//
// kolory znaku - jasne
//
#define LIGHT_BLUE		FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define LIGHT_GREEN		FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define LIGHT_RED		FOREGROUND_INTENSITY | FOREGROUND_RED
#define LIGHT_YELLOW	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define LIGHT_PURPLE	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define LIGHT_AQUA		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
#define BRIGHT_WHITE		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

static void set_foreground(int fore_color);
static void set_background(int back_color);
static void hide_cursor(void);
static void show_cursor(void);
static void set_cursor_visibility(int visible);
static void set_screen_size(int width, int height);
static void sleep(int time_ms);
static int get_time(void);
static void gotoxy(int x, int y);


static void set_screen_size(int width, int height)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT r;
	COORD      c;
	houtput = GetStdHandle(STD_OUTPUT_HANDLE);

	set_background(BLACK);
	set_foreground(WHITE);

	if (!GetConsoleScreenBufferInfo(houtput, &csbi))
		exit(1);

	c.X = width;
	c.Y = height;
	BOOL a = SetConsoleScreenBufferSize(houtput, c);

	r.Left = r.Top = 0;
	r.Right = width - 1;
	r.Bottom = height - 1;
	BOOL b = SetConsoleWindowInfo(houtput, TRUE, &r);
}

static void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	BOOL b = SetConsoleCursorPosition(houtput, pos);
}

static void set_cursor_visibility(int visible)
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(houtput, &cci);
	cci.bVisible = visible;
	SetConsoleCursorInfo(houtput, &cci);
}

static void show_cursor(void)
{
	set_cursor_visibility(1);
}

static void hide_cursor(void)
{
	set_cursor_visibility(0);
}

static void set_background(int back_color)
{
	current_background = back_color & 0x0F;
	SetConsoleTextAttribute(houtput, (current_background << 4) | current_foreground);
}

static void set_foreground(int fore_color)
{
	current_foreground = (fore_color & 0x0F);
	SetConsoleTextAttribute(houtput, (current_background << 4) | current_foreground);
}

static int get_time(void)
{
	return (int)GetTickCount64();
}

static void sleep(int time_ms)
{
	Sleep(time_ms);
}

