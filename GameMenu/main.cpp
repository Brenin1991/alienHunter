#include<stdio.h>
#include <iostream>
#include<conio.h>
#include"includes/conio2.c"
#include<windows.h>
#include<stdlib.h>
#include <locale.h>
#include "includes/mmsystem.h"

#define acima 80
#define abaixo 72
#define enter 13

/*

    HARD ROCK STUDIO

*/

void Menu(int X,int Y);
int main();
void Aux_main();
char nome[10];
int pontos = 0;
char c , charPontos[50];
FILE *fp, *fpsalva;

void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

//Função que remove o cursor do console
void HideCursor(){
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

//Função que configura o tamanho da tela e do buffer
void Configtela(){
    COORD outbuff;
    // tem que ser 1 maior que o tamanho X
    outbuff.X = 100;
    // tem que ser 1 maior que o tamanho Y
    outbuff.Y = 30;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(hConsole, outbuff);
    Sleep(130);
    SMALL_RECT windowSize = {0, 0, 99, 29};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

//Logo Menu
int Logo_Alien[10][20]{
    0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,1,0,0,1,
    1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,
    1,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1,1,0,1,
    1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,1,
    1,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,0,0,1,
};

//Logo Menu
int Logo_Hunter[10][30]{
    2,0,0,2,0,2,0,0,2,0,2,0,0,2,0,2,2,2,0,2,2,2,2,0,2,2,2,0,0,0,
    2,2,2,2,0,2,0,0,2,0,2,0,0,2,0,0,2,0,0,2,0,0,0,0,2,0,0,2,0,0,
    2,0,0,2,0,2,0,0,2,0,2,2,0,2,0,0,2,0,0,2,2,2,0,0,2,2,2,0,0,0,
    2,0,0,2,0,2,0,0,2,0,2,0,2,2,0,0,2,0,0,2,0,0,0,0,2,0,2,0,0,0,
    2,0,0,2,0,0,2,2,0,0,2,0,0,2,1,0,2,0,0,2,2,2,2,0,2,0,0,2,0,0,
};

//Logo Score
int Logo_Score[10][25]{
    0,2,2,2,0, 0,2,2,2,0, 0,2,2,0,0, 2,2,2,0,0, 2,2,2,2,0,
    2,0,0,0,0, 2,0,0,0,0, 2,0,0,2,0, 2,0,0,2,0, 2,0,0,0,0,
    0,2,2,0,0, 2,0,0,0,0, 2,0,0,2,0, 2,2,2,0,0, 2,2,2,0,0,
    0,0,0,2,0, 2,0,0,0,0, 2,0,0,2,0, 2,0,2,0,0, 2,0,0,0,0,
    2,2,2,0,0, 0,2,2,2,0, 0,2,2,0,0, 2,0,0,2,0, 2,2,2,2,0,
};

//Logo crédito
int Logo_Credits[10][31]{
    0,2,2,2,0, 2,2,2,0,0, 2,2,2,2,0, 2,2,2,0,0, 2,0, 2,2,2,0, 0,2,2,2,0,
    2,0,0,0,0, 2,0,0,2,0, 2,0,0,0,0, 2,0,0,2,0, 2,0, 0,2,0,0, 2,0,0,0,0,
    2,0,0,0,0, 2,2,2,0,0, 2,2,2,0,0, 2,0,0,2,0, 2,0, 0,2,0,0, 0,2,2,0,0,
    2,0,0,0,0, 2,0,2,0,0, 2,0,0,0,0, 2,0,0,2,0, 2,0, 0,2,0,0, 0,0,0,2,0,
    0,2,2,2,0, 2,0,0,2,0, 2,2,2,2,0, 2,2,2,0,0, 2,0, 0,2,0,0, 2,2,2,0,0,
};

//Logo Opições
int Logo_Options[10][31]{
    0,2,2,0,0, 2,2,2,0,0, 2,2,2,0, 2,0, 0,2,2,0,0, 2,0,0,2,0, 0,2,2,2,0,
    2,0,0,2,0, 2,0,0,2,0, 0,2,0,0, 2,0, 2,0,0,2,0, 2,0,0,2,0, 2,0,0,0,0,
    2,0,0,2,0, 2,2,2,0,0, 0,2,0,0, 2,0, 2,0,0,2,0, 2,2,0,2,0, 0,2,2,0,0,
    2,0,0,2,0, 2,0,0,0,0, 0,2,0,0, 2,0, 2,0,0,2,0, 2,0,2,2,0, 0,0,0,2,0,
    0,2,2,0,0, 2,0,0,0,0, 0,2,0,0, 2,0, 0,2,2,0,0, 2,0,0,2,0, 2,2,2,0,0,
};

int Logo_GM[10][43]{
    0,2,2,0,0, 0,2,2,0,0, 2,0,0,2,0, 2,2,2,2,0, 0,0,0, 0,2,2,0,0, 2,0,0,2,0, 2,2,2,2,0, 2,2,2,0,0,
    2,0,0,0,0, 2,0,0,2,0, 2,2,2,2,0, 2,0,0,0,0, 0,0,0, 2,0,0,2,0, 2,0,0,2,0, 2,0,0,0,0, 2,0,0,2,0,
    2,0,2,2,0, 2,2,2,2,0, 2,0,0,2,0, 2,2,2,0,0, 0,0,0, 2,0,0,2,0, 2,0,0,2,0, 2,2,2,0,0, 2,2,2,0,0,
    2,0,0,2,0, 2,0,0,2,0, 2,0,0,2,0, 2,0,0,0,0, 0,0,0, 2,0,0,2,0, 2,0,0,2,0, 2,0,0,0,0, 2,0,2,0,0,
    0,2,2,0,0, 2,0,0,2,0, 2,0,0,2,0, 2,2,2,2,0, 0,0,0, 0,2,2,0,0, 0,2,2,0,0, 2,2,2,2,0, 2,0,0,2,0,
};

//Menu opções
void Options_Menu(int X,int Y){
    system("cls");
    textbackground(BLACK);
    system("cls");
    for(int x = 0;x<31;x++){
        for(int y = 0;y<5;y++){
            if(Logo_Options[y][x] == 2){
                textbackground(YELLOW);
                printxy(x * 2 + 20, y + 2,"  ");
            }
        }
    }
    textbackground(BLACK);
    textcolor(WHITE);
	printxy(X, Y++,"                                ");
    printxy(X, Y++,"                                ");
    printxy(X, Y++,"     DIFFICULTY");
	printxy(X, Y++,"     AUDIO SETTINGS");
	printxy(X, Y++,"     VIDEO SETTINGS");
	printxy(X, Y++,"     SCORE SETTINGS");
	printxy(X, Y++,"     ABOUT SOFTWARE");
	printxy(X, Y++,"     BACK WITHOUT SAVE");
	printxy(X, Y++,"     BACK AND SAVE");
    printxy(X, Y++,"                                ");
    textcolor(WHITE); printxy(35, 28,"® 2017 - HARD ROCK STUDIO");
}

//Função Escolha
int Options_Escolha(int y){
	system("cls");
	system("COLOR 0");
	int X=5,Y=5, op;
	switch(y){

		case 15:  break;

		case 16:  break;

		case 17:  break;

		case 18:  break;

		case 19:  break;

		case 20:  Aux_main(); break;

		case 21:  Aux_main(); break;

	}
    textbackground(BLACK);
	system("pause");
}

//Função opções
void Options(){
    int ch,x,y,X,Y;
	x=40; y=15; X=40; Y=13;
	while(1){

	Options_Menu(X,Y);//variaveis em maiusculas
	gotoxy(x,y); //variaveis em minusculas
	textcolor(LIGHT_GREEN); printf(">\n");

	while(1){
		ch=getch();
			if(ch==224){
	            switch ( getch() ){
					case acima :
					    if(y<21){
							gotoxy(x,y);
							printf(" ");
							gotoxy(x,++y);
							textcolor(LIGHT_GREEN); printf(">");
							PlaySound("sons\\select.wav", NULL, SND_SYNC);
					} break;


					case abaixo :
						if(y>15){
							gotoxy(x,y);
							printf(" ");
							gotoxy(x,--y);
							textcolor(LIGHT_GREEN); printf(">");
							PlaySound("sons\\select.wav", NULL, SND_SYNC);
					}  break ;

					defalte : break;
				}

			}
			else if(ch==enter){
                    PlaySound("sons\\enter.wav", NULL, SND_SYNC);
				if(Options_Escolha(y)==1){
					//goto fim;
				} break;
			}

	}
    }

}

//Função salvar score
void SaveScore(){
    char Linha[100];
    char *result;

    int i;
    fp = fopen("data\\localScore.sv", "a+");
    i = 1;
    while (!feof(fp)){
      result = fgets(Linha, 100, fp);
    }
    fclose(fp);
    fpsalva = fopen("data\\globalScore.sv", "a");
    fprintf(fpsalva,"\t\t\t%s\t\t\t\t\t\t%s", nome, Linha);
    fclose(fpsalva);
}

//Função logo inicial
void Logo(){
    textbackground(BLACK);
    system("cls");
    for(int x = 0;x<20;x++){
        for(int y = 0;y<5;y++){
            if(Logo_Alien[y][x] == 1){
                textbackground(RED);
                printxy(x * 2 + 30, y + 2,"  ");
            }
        }
    }

    for(int x = 0;x<30;x++){
        for(int y = 0;y<5;y++){
            if(Logo_Hunter[y][x] == 2){
                textbackground(LIGHT_MAGENTA);
                printxy(x * 2 + 22, y + 8,"  ");
            }
        }
    }
    textbackground(BLACK);
}

//Função cadastro
void Cadastro(){
    textcolor(YELLOW);
    printxy(35, 15,"NICKNAME : ");
    textbackground(WHITE);
    textcolor(BLACK);
    printxy(46, 15,"                ");
    gotoxy(46,15);
    gets(nome);
}

//Função menu
void Menu(int X,int Y){
    fp = fopen("data\\localScore.sv", "a+");
    int cont = 0;
    system("cls");
    textbackground(BLACK);
    textcolor(WHITE);
    Logo();
    PlaySound("sons\\open.wav", NULL, SND_SYNC);
	printxy(X, Y++,"                                ");
    printxy(X, Y++,"                                ");
	printxy(X, Y++,"     PLAY");
	printxy(X, Y++,"     SCORE");
	printxy(X, Y++,"     OPTIONS");
	printxy(X, Y++,"     CREDITS");
	printxy(X, Y++,"     CHANGE USER");
	printxy(X, Y++,"     EXIT");
    printxy(X, Y++,"                                ");
    textcolor(YELLOW);
    printxy(45, 22,"LOGGED IN WITH");
    textcolor(WHITE);
    gotoxy(45,23); printf("%s -", nome);
    gotoxy(55,23);
    do{
        c = fgetc(fp);
        if (c != EOF){
                printf("%c", c);
        }
        charPontos[cont] = c;
        cont += 1;
    }while(c != EOF);
    fclose(fp);
    textcolor(WHITE); printxy(35, 28,"® 2017 - HARD ROCK STUDIO");
}

//Função Score
void Score(){
    fpsalva = fopen("data\\globalScore.sv", "a+");
    textbackground(BLACK);
    system("cls");
    for(int x = 0;x<25;x++){
        for(int y = 0;y<5;y++){
            if(Logo_Score[y][x] == 2){
                textbackground(YELLOW);
                printxy(x * 2 + 25, y + 2,"  ");
            }
        }
    }
    textbackground(BLACK);
    textbackground(CYAN); textcolor(YELLOW); printxy(23,10,"PLAYER");
    textbackground(CYAN); textcolor(YELLOW); printxy(70,10,"POINTS\n");
    textbackground(BLACK); textcolor(WHITE);
    do{
        c = fgetc(fpsalva);
        if (c != EOF){
                printf("%c", c);
        }

    }while(c != EOF);
    fclose(fp);
    textbackground(BLACK);
    textcolor(BLACK);
}

//Função Créditos
void Creditos(){
    textbackground(BLACK);
    system("cls");
    for(int x = 0;x<30;x++){
        for(int y = 0;y<5;y++){
            if(Logo_Credits[y][x] == 2){
                textbackground(LIGHT_GREEN);
                printxy(x * 2 + 20, y + 2,"  ");
            }
        }
    }
    textbackground(BLACK);
    textbackground(CYAN); textcolor(YELLOW); printxy(35,10,"CODE, GRAPHICS & DESIGN");
    textbackground(BLACK); textcolor(WHITE); printxy(35,12,"BRENO CAMPOS RIBEIRO FROM HARD ROCK STUDIO");
    textbackground(BLACK); textcolor(LIGHT_RED); printxy(35,14,"WEBSITE: http://hardrockstudio.cf/");
    textbackground(CYAN); textcolor(YELLOW); printxy(35,16,"SFX & SOUND");
    textbackground(BLACK); textcolor(WHITE); printxy(35,18,"FREE SOUND EFFECTS 2017/ SOUND EFFECTS 2016/ BIT8MUSIC 2008");
    textbackground(BLACK); textcolor(WHITE); printxy(15,22,"THIS GAME IS THE FIRST WORK OF CONCLUSION OF THE YEAR OF THE 2017 'IT' COURSE.");
    textbackground(BLACK); textcolor(WHITE); printxy(15,23,"INITIATED 15/11/2017 AND PRESENTED A DEMO 01/12/2017 (IFPR CAMPUS PARANAGUÁ).");
    textbackground(BLACK); textcolor(WHITE); printxy(15,24,"LESSON BY");textbackground(BLACK); textcolor(LIGHT_RED); printxy(26,24,"GIL EDUARDO DE ANDRADE. ");
    textbackground(BLACK); textcolor(LIGHT_CYAN); printxy(15,25,"http://gileduardo.com.br/");
    textbackground(BLACK); textcolor(BLACK);
}

//Função Escolha
int Escolha(int y){
	system("cls");
	system("COLOR 0");
	int X=5,Y=5, op;
	switch(y){

		case 15:
		    textbackground(BLACK);
            textcolor(BLACK);
		    system("game.exe");
            fp = fopen("data\\localScore.sv", "a+");
		    system("cls");
            textbackground(BLACK);
            system("cls");
            for(int x = 0;x<43;x++){
                for(int y = 0;y<5;y++){
                    if(Logo_GM[y][x] == 2){
                        textbackground(LIGHT_RED);
                        printxy(x * 2 + 10, y + 2,"  ");
                    }
                }
            }
            textcolor(WHITE);
            textbackground(BLACK);
            textcolor(LIGHT_RED); printxy(40, 10,"YOUR SCORE : ");
            textcolor(WHITE);
            gotoxy(54,10);
            do{
                c = fgetc(fp);
                if (c != EOF){
                        printf("%c", c);
                }
                //charPontos[cont] = c;
                //cont += 1;
            }while(c != EOF);
            fclose(fp);

            textcolor(YELLOW);
            printxy(35, 15,"SAVE SCORE? (1 - YES) (2 - NO) ");
            textbackground(WHITE);
            textcolor(BLACK);
            gotoxy(49,16);
            scanf("%i", &op);
            if(op == 1){
                SaveScore();
            }
            break;

		case 16: Score(); break;

		case 17: Options(); break;

		case 18: Creditos(); break;

		case 19: main(); break;

		case 20:
		    fp = fopen("data\\localScore.sv", "w");
            fclose(fp);
		    exit(1);
        break;

	}
    textbackground(BLACK);
	system("pause");
}

int main(){
    setlocale(LC_ALL, "Portuguese");
	system("TITLE Alien Hunter - Menu");
	fullscreen();
	HideCursor();
	Configtela();

    Cadastro();
    Aux_main();


}

void Aux_main(){
    int ch,x,y,X,Y;
	x=40; y=15; X=40; Y=13;

    while(1){

	Menu(X,Y);//variaveis em maiusculas
	gotoxy(x,y); //variaveis em minusculas
	textcolor(LIGHT_GREEN); printf(">\n");

	while(1){
		ch=getch();
			if(ch==224){
	            switch ( getch() ){
					case acima :
					    if(y<20){
							gotoxy(x,y);
							printf(" ");
							gotoxy(x,++y);
							textcolor(LIGHT_GREEN); printf(">");
							PlaySound("sons\\select.wav", NULL, SND_SYNC);
					} break;


					case abaixo :
						if(y>15){
							gotoxy(x,y);
							printf(" ");
							gotoxy(x,--y);
							textcolor(LIGHT_GREEN); printf(">");
							PlaySound("sons\\select.wav", NULL, SND_SYNC);
					}  break ;

					defalte : break;
				}

			}
			else if(ch==enter){
                    PlaySound("sons\\enter.wav", NULL, SND_SYNC);
				if(Escolha(y)==1){
					goto fim;
				} break;
			}
	}
	system ("cls");
    }
    fim:
    system("pause");
}
