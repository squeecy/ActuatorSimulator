#define NOMINMAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = {0 , 0}; 

	std::cout.flush();

	if(!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		abort();
	}

	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;

	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
}


void ClearScreen()
{
	COORD cursorPosition; cursorPosition.X = 0; cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	std::cout.flush();
}
