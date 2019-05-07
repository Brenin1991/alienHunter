#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>

// -------------------------------------------------------------------------

enum DOS_COLORS {
     BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
     LIGHT_GRAY, DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN,
     LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE
};

// -------------------------------------------------------------------------

void gotoxy(int coluna, int linha) {
     COORD point;
     point.X = coluna;
     point.Y = linha;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

// -------------------------------------------------------------------------

void textcolor (DOS_COLORS iColor) {
     HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
     BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
     bufferInfo.wAttributes &= 0x00F0;
     SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= iColor);
}

// -------------------------------------------------------------------------

void textbackground (DOS_COLORS iColor) {
     HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
     BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
     bufferInfo.wAttributes &= 0x000F;
     SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= (iColor << 4));
}

// -------------------------------------------------------------------------

void wait ( float seconds ) {
     clock_t endwait;
     endwait = clock () + seconds * CLOCKS_PER_SEC ;
     while (clock() < endwait) {}
}

// -------------------------------------------------------------------------

void printxy(int coluna, int linha, char texto[]) {
     
     gotoxy(coluna, linha);
     printf(texto);

}
