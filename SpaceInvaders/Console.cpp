#include "Console.h"

#include <windows.h>

void set_size(int wid, int hei)
{
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD buff_size = {(short)wid, (short)hei};
    SetConsoleScreenBufferSize(h_std, buff_size);
    SMALL_RECT window_size = {0, 0, (short)(wid - 1), (short)(hei - 1)};
    SetConsoleWindowInfo(h_std, TRUE, &window_size);
}

void set_fontsize(int font_wid, int font_hei)
{
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontinfo;
    fontinfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(h_std, FALSE, &fontinfo);
    fontinfo.dwFontSize.Y = font_hei;
    fontinfo.dwFontSize.X = font_wid;
    SetCurrentConsoleFontEx(h_std, FALSE, &fontinfo);
}

void set_color(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | fg);
}

void gotoxy(short x, short y)
{
    HANDLE h_std = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(h_std, pos);
}

void hide_cursor()
{
   HANDLE Hcon = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(Hcon, &info);
}
