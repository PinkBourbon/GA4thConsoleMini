
#include "__console.h"

// 핸들은 전역으로 쓸 것이다. 이 파일 범위를 벗어나지 않는다.
HANDLE g_hConsole;


BOOL Initialize()
{
	// 콘솔게임용 핸들 얻어옴
	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (g_hConsole == INVALID_HANDLE_VALUE)
	{
		printf("Err, INVALID_HANDLE_VALUE\n");

		return FALSE;
	}

	return TRUE;
}

//// 화면 채우기
void LHFillScreen(int ch)
{
	int   ix, iy;
	COORD cdPos;

	for (iy=0; iy < 24; iy++)
	{
		for (ix=0; ix < 80; ix++)
		{
			cdPos.X = ix;
			cdPos.Y = iy;
			SetConsoleCursorPosition(g_hConsole, cdPos);
			printf("%c", ch);
			//putchar(ch);
		}	
	}
}

//// 좌표에 문자열 출력
void LHFillScreenEx(const char* pStr)
{
	int   ix, iy;
	COORD cdPos;
	int	  nStrlen = strlen(pStr);

	for (iy=0; iy < 24; iy++)
	{
		for (ix=0; ix < 80; ix+=nStrlen)
		{
			cdPos.X = ix;
			cdPos.Y = iy;
			SetConsoleCursorPosition(g_hConsole, cdPos);
			printf("%s", pStr);			
		}	
	}	
}

//// 커서 위치 설정
void LHLocateCursor(int x, int y)
{
	COORD cdPos = { x, y };

	SetConsoleCursorPosition(g_hConsole, cdPos);
}


//// 좌표에 문자 출력
void LHPutChar(int x, int y, char ch)
{
	COORD cdPos = { x, y };

	SetConsoleCursorPosition(g_hConsole, cdPos);
	printf("%c", ch);
	//putchar(ch);
}

//// 좌표에 문자 출력 ( 컬러속성설정)
void LHPutAttrChar(int x, int y, char ch, WORD bg, WORD fg)
{
	DWORD dwCharsWritten;
	COORD cdFill = { x, y};
	WORD  wColor = bg | fg;
	
	FillConsoleOutputAttribute(g_hConsole, wColor, 1,	cdFill,	&dwCharsWritten);
	FillConsoleOutputCharacter(g_hConsole, ch, 1, cdFill, &dwCharsWritten);
}

//// 좌표에 문자열 출력 ( 컬러속성설정 )
void LHPutAttrCharEx(int x, int y, const char* pStr, WORD bg, WORD fg)
{
	DWORD dwCharsWritten;
	COORD cdFill = { x, y };
	WORD  wColor = bg | fg;
	int   nStrlen = strlen(pStr);
	
	FillConsoleOutputAttribute(g_hConsole, wColor, nStrlen, cdFill, &dwCharsWritten);
	WriteConsoleOutputCharacter(g_hConsole, pStr, nStrlen, cdFill, &dwCharsWritten);
}

//// 전체 문자 컬러 속성 설정
void LHSetAttrChar(WORD bg, WORD fg)
{
	WORD wColor = bg | fg;
	SetConsoleTextAttribute(g_hConsole, wColor);
}

//// 커서 상태 설정
void LHSetCursor(DWORD dwSize, BOOL bVisible)
{	
	CONSOLE_CURSOR_INFO curInfo;

	curInfo.bVisible = bVisible;
	curInfo.dwSize = dwSize;
	SetConsoleCursorInfo(g_hConsole, &curInfo);
}