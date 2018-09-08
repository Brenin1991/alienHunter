#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <SDL_image.h>
#include "string"
#include <SDL_mixer.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>
#include <windows.h>

/*

    BR3N0 C4MP05 R1BEIR0

*/

//variariveis globais
//Sons
int frequencia = 22050;
Uint16 formato = AUDIO_S16SYS;
int canal = 2;//1 - mono & 2 - estereo
int buffer = 4096;
int Som = 0;
//Arquivo
FILE *fp;

//Valores elementos(CONFIGURAÇÕES).
int tamanhoSprites = 40;
float velocidadeInimigo = 0.5;
float velocidadePlayer = 0.8;
float tamanhoBala = 10;
float velocidadeBala = 0.2;
int Powercont = 0;
float PosX = -100;
float PosY = -100;
//cor da bala
int RED = 255;
int GREEN = 0;
int BLUE = 0;
float VelBala = 3;

//float velocidadePlayer = 0.8;

//Estrutura do power up.
struct powerUp{
    int tipo;
    float posX = PosX;
    float posY = PosY;
    float tam = tamanhoSprites - 10;
    int cont = 0;
};
powerUp estruturaPowerUp[3];

//Estrutura do HUD.
struct hud{
    float xHud = 20;
    float yHud = 10;
    float hudTamX = 200;
    float hudTamY = 10;
};
hud estruturaHud;

//Estrutura do inimigo "Ovni".
struct ovni{
    float inimigoX =  50 + ( rand() % 300 );;
    float inimigoY =  50 + ( rand() % 200 );;
    float inimigoCA = tamanhoSprites;
    float velx = velocidadeInimigo;
    float vely = velocidadeInimigo;
    float balaX = inimigoX + 15;
    float balaY = inimigoY;
    float balaTam = tamanhoBala;
    float velxbala = velocidadeBala;
    float velybala = velocidadeBala;
};
ovni estruturaOvni;

//Estrutura do player "Nave".
struct player{
    float vel = velocidadePlayer;
    float personX = 300;
    float personY = 350;
    float personComp = tamanhoSprites;
    float personAlt = tamanhoSprites;
    float pontos;
    bool esquerda = false;
    bool direita = false;
    bool cima = false;
    bool baixo = false;
    bool tiro = false;
    float balaX = personX + 15;
    float balaY = personY;
    float balaTam = tamanhoBala;
    bool cont = false;
    int red = RED;
    int green = GREEN;
    int blue = BLUE;
    float velBala = VelBala;
};
player estruturaPlayer;

//Estrutura do inimigo "Asteróide".
struct inimigo{
    float inimigoX =  50 + ( rand() % 300 );
    float inimigoY =  50 + ( rand() % 200 );
    float inimigoCA = tamanhoSprites;
    float velx = velocidadeInimigo;
    float vely = velocidadeInimigo;
};
inimigo estruturaInimigo[4];

//Função de colisao (A = Elemeto 1 : exemplo "player" & B é o elemento 2 exemplo : "inimogo").
bool colisao(float Ax, float Ay, float Acomp, float Aalt, float Bx, float By, float Bcomp, float Balt){
    if(Ay+Aalt < By) return false;
    else if(Ay > By+Balt)return false;
    else if(Ax+Acomp < Bx)return false;
    else if(Ax > Bx+Bcomp)return false;

    return true;//houve colisao.
}

//Função que instancia o player "Nave".
int playerSpawn(){
    int conf = 0;
    //logica de tiro.
    if(estruturaPlayer.tiro == true){
        estruturaPlayer.balaY -= estruturaPlayer.velBala;
        conf = 1;//retorna 1 (Som de tiro).
        if(estruturaPlayer.balaY <= 0){
            estruturaPlayer.balaY = estruturaPlayer.personY;
        }
    }
    else if(estruturaPlayer.tiro == false){
        estruturaPlayer.balaY = estruturaPlayer.personY;
    }
    //movimento do player.
    if(estruturaPlayer.esquerda == true){
        estruturaPlayer.personX -= estruturaPlayer.vel;
        estruturaPlayer.balaX -=estruturaPlayer.vel;
    }
    else if(estruturaPlayer.direita == true){
        estruturaPlayer.personX += estruturaPlayer.vel;
        estruturaPlayer.balaX +=estruturaPlayer.vel;
    }
    else if(estruturaPlayer.cima == true){
        estruturaPlayer.personY -= estruturaPlayer.vel;
        estruturaPlayer.balaY -=estruturaPlayer.vel;
    }
    else if(estruturaPlayer.baixo == true){
        estruturaPlayer.personY += estruturaPlayer.vel;
        estruturaPlayer.balaY +=estruturaPlayer.vel;
    }
    //colisão do player.
    if(estruturaPlayer.personX < 0){
        estruturaPlayer.personX = 0;
        estruturaPlayer.balaX = 15;
    }
    else if(estruturaPlayer.personX+estruturaPlayer.personComp > 600){
        estruturaPlayer.personX = 600 - estruturaPlayer.personComp;
        estruturaPlayer.balaX = 585 - estruturaPlayer.balaTam;
    }
    else if(estruturaPlayer.personY < 0){
        estruturaPlayer.personY = 0;
        estruturaPlayer.balaY = 0;
    }
    else if(estruturaPlayer.personY+estruturaPlayer.personAlt > 400){
        estruturaPlayer.personY = 400 - estruturaPlayer.personAlt;
        estruturaPlayer.balaY = 400 - estruturaPlayer.balaTam;
    }
    return conf;//Conf retorna som (1 - Som de tiro ou 2 - explosão).
}

//Função que instancia o inimigo "Asteróide".
int inimigoSpawn(int pos){
    int conf = 0;
    //movimento do inimigo.
    estruturaInimigo[pos].inimigoX -= estruturaInimigo[pos].velx;
    estruturaInimigo[pos].inimigoY -= estruturaInimigo[pos].vely;

    if(estruturaInimigo[pos].inimigoX < 0){
        estruturaInimigo[pos].velx = -estruturaInimigo[pos].velx;
    }
    else if(estruturaInimigo[pos].inimigoX+estruturaInimigo[pos].inimigoCA > 600){
        estruturaInimigo[pos].velx = -estruturaInimigo[pos].velx;
    }
    else if(estruturaInimigo[pos].inimigoY < 0){
        estruturaInimigo[pos].vely = -estruturaInimigo[pos].vely;
    }
    else if(estruturaInimigo[pos].inimigoY+estruturaInimigo[pos].inimigoCA > 400){
        estruturaInimigo[pos].vely = -estruturaInimigo[pos].vely;
    }

    //dano do inimigo sobre o player.
    if(colisao(estruturaInimigo[pos].inimigoX, estruturaInimigo[pos].inimigoY, estruturaInimigo[pos].inimigoCA, estruturaInimigo[pos].inimigoCA, estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt)){
        conf = 2;
        estruturaInimigo[pos].vely = -estruturaInimigo[pos].vely;
        estruturaHud.hudTamX -=1;
    }
    //dano que inimigo toma do player.
    if(colisao(estruturaInimigo[pos].inimigoX, estruturaInimigo[pos].inimigoY, estruturaInimigo[pos].inimigoCA, estruturaInimigo[pos].inimigoCA, estruturaPlayer.balaX, estruturaPlayer.balaY, estruturaPlayer.balaTam, estruturaPlayer.balaTam)){
        conf = 2;
        estruturaInimigo[pos].inimigoX = 50 + ( rand() % 300 );
        estruturaInimigo[pos].inimigoY = 50 + ( rand() % 200 );
        estruturaPlayer.pontos += 10;
        Powercont += 10;
    }
    return conf;
}

//Função que instancia o inimigo "Ovni".
int ovniSpawn(){
    int conf = 0;
    //movimento do inimigo.
    estruturaOvni.inimigoX += estruturaOvni.velx;
    estruturaOvni.inimigoY += estruturaOvni.vely;
    estruturaOvni.balaX += estruturaOvni.velx;
    estruturaOvni.balaY += estruturaOvni.vely;


    //logica do inimigo.
    if(estruturaOvni.inimigoX < 0){
        estruturaOvni.velx = -estruturaOvni.velx;
        estruturaOvni.velxbala = estruturaOvni.velx;
        estruturaPlayer.cont = true;
    }
    else if(estruturaOvni.inimigoX+estruturaOvni.inimigoCA > 600){
        estruturaOvni.velx = -estruturaOvni.velx;
        estruturaOvni.velxbala = estruturaOvni.velx;
        estruturaPlayer.cont = false;
    }
    else if(estruturaOvni.inimigoY < 0){
        estruturaOvni.vely = -estruturaOvni.vely;
        estruturaOvni.velybala = estruturaOvni.vely;
        estruturaPlayer.cont = true;
    }
    else if(estruturaOvni.inimigoY+estruturaOvni.inimigoCA > 400){
        estruturaOvni.vely = -estruturaOvni.vely;
        estruturaOvni.velybala = estruturaOvni.vely;
        estruturaPlayer.cont = false;
    }
    //dano do inimigo sobre o player.
    if(colisao(estruturaOvni.inimigoX, estruturaOvni.inimigoY, estruturaOvni.inimigoCA, estruturaOvni.inimigoCA, estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt)){
        estruturaOvni.vely = -estruturaOvni.vely;
        estruturaHud.hudTamX -=1;
        conf = 2;
    }
    //dano que inimigo toma do player.
    if(colisao(estruturaOvni.inimigoX, estruturaOvni.inimigoY, estruturaOvni.inimigoCA, estruturaOvni.inimigoCA, estruturaPlayer.balaX, estruturaPlayer.balaY, estruturaPlayer.balaTam, estruturaPlayer.balaTam)){
        conf = 2;
        estruturaOvni.inimigoX = 50 + ( rand() % 300 );
        estruturaOvni.inimigoY = 50 + ( rand() % 200 );
        estruturaOvni.balaX = estruturaOvni.inimigoX + 15;
        estruturaOvni.balaY = estruturaOvni.inimigoY;
        estruturaPlayer.pontos += 10;
        Powercont += 10;
    }
    if(colisao(estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt, estruturaOvni.balaX, estruturaOvni.balaY, estruturaOvni.balaTam, estruturaOvni.balaTam)){
        estruturaHud.hudTamX -= 1;
    }
    //logica de tiro do inimigo.
    if(estruturaPlayer.cont == true){
        estruturaOvni.balaY += 1;
        conf = 1;
        if(estruturaOvni.balaY >= 400){
          estruturaOvni.balaY = estruturaOvni.inimigoY;
        }
    }
    else if(estruturaPlayer.cont == false){
        estruturaOvni.balaY = estruturaOvni.inimigoY;
    }
    return conf;
}

int powerUpSpawn(){
    int pos;
    int confSom;
    bool conf = false;
     pos = 0 + ( rand() % 3 );
    if(estruturaPowerUp[pos].tipo == 1){
        if(Powercont > 100){
            conf = true;
        }
            if(conf == true){
                //movimento do power up.
                estruturaPowerUp[pos].posX = 50 + ( rand() % 300 );
                estruturaPowerUp[pos].posY = 50 + ( rand() % 200 );
                Powercont = 0;
            }
        //quando a posição do power up é igual do player.
        if(colisao(estruturaPowerUp[pos].posX, estruturaPowerUp[pos].posY, estruturaPowerUp[pos].tam, estruturaPowerUp[pos].tam, estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt)){
            estruturaPowerUp[pos].posX = PosX;
            estruturaPowerUp[pos].posY = PosY;
            estruturaHud.hudTamX = estruturaHud.hudTamX + 30;
            confSom = 1;
        }
    }
    if(estruturaPowerUp[pos].tipo == 2){
        if(Powercont > 100){
            conf = true;
        }
            if(conf == true){
                //movimento do power up.
                estruturaPowerUp[pos].posX = 50 + ( rand() % 300 );
                estruturaPowerUp[pos].posY = 50 + ( rand() % 200 );
                Powercont = 0;
            }
        //quando a posição do power up é igual do player.
        if(colisao(estruturaPowerUp[pos].posX, estruturaPowerUp[pos].posY, estruturaPowerUp[pos].tam, estruturaPowerUp[pos].tam, estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt)){
            estruturaPowerUp[pos].posX = PosX;
            estruturaPowerUp[pos].posY = PosY;
            estruturaPlayer.red= RED, estruturaPlayer.green = GREEN, estruturaPlayer.blue = BLUE;
            estruturaPlayer.velBala = VelBala;
            estruturaPlayer.vel = 2;
            confSom = 2;
        }
    }
    if(estruturaPowerUp[pos].tipo == 3){
        if(Powercont > 100){
            conf = true;
        }
            if(conf == true){
                //movimento do power up.
                estruturaPowerUp[pos].posX = 50 + ( rand() % 300 );
                estruturaPowerUp[pos].posY = 50 + ( rand() % 200 );
                Powercont = 0;
            }
        //quando a posição do power up é igual do player.
        if(colisao(estruturaPowerUp[pos].posX, estruturaPowerUp[pos].posY, estruturaPowerUp[pos].tam, estruturaPowerUp[pos].tam, estruturaPlayer.personX, estruturaPlayer.personY, estruturaPlayer.personComp, estruturaPlayer.personAlt)){
            estruturaPowerUp[pos].posX = PosX;
            estruturaPowerUp[pos].posY = PosY;
            estruturaPlayer.red= 0, estruturaPlayer.green = 0, estruturaPlayer.blue = 255;
            estruturaPlayer.velBala = 10;
            estruturaPlayer.vel = velocidadePlayer;
            confSom = 3;
        }

    }
    return confSom;
}

//Função de carregar textura
GLuint loadTexture(const std::string&fileName){
    SDL_Surface *imagem = IMG_Load(fileName.c_str());//carrega imagem
    SDL_DisplayFormatAlpha(imagem);//formato alpha
    unsigned object(0);//cria textura
    glGenTextures(1, &object);//gera textura
    glBindTexture(GL_TEXTURE_2D, object);//tipo de textura, 2d

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagem->w, imagem->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem->pixels);

    SDL_FreeSurface(imagem);
    return object;
}

int main(int argc, char* args[]){
    //abre o arquivo "localScore.sv".
    fp = fopen("data//localScore.sv", "w");
    if(fp == NULL) {
        printf("Erro ao abrir o arquivo!");
    }

    //audio - Carrega músicas.
    Mix_OpenAudio(frequencia, formato, canal, buffer);

    Mix_Music *musica;
    musica = Mix_LoadMUS("sons/sound.wav");
    Mix_PlayMusic(musica, 10);

    Mix_Chunk *tiro;
    tiro = Mix_LoadWAV("sons/gun.wav");

    Mix_Chunk *explosao;
    explosao = Mix_LoadWAV("sons/explosion.wav");

    Mix_Chunk *morte;
    morte = Mix_LoadWAV("sons/morte.wav");

    Mix_Chunk *powerUp_med;
    powerUp_med = Mix_LoadWAV("sons/powerUp_med.wav");

    Mix_Chunk *powerUp_plasma;
    powerUp_plasma = Mix_LoadWAV("sons/powerUp_plasma.wav");

    Mix_Chunk *powerUp_speed;
    powerUp_speed = Mix_LoadWAV("sons/powerUp_speed.wav");


    //configuração de tela.
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface *tela;
    SDL_Surface *icone;

    icone = IMG_Load("sprites/ovni.png");

    //memoria
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, -1);

    //nome
    SDL_WM_SetCaption("Alien Hunter", NULL);

    //tamanho da janela.
    tela = SDL_SetVideoMode(800, 600, 32, SDL_OPENGLBLIT);
    SDL_WM_SetIcon(icone, NULL);

    //cor
    glClearColor(0,0,0,1);

    //area exibida.
    glViewport(0,0,800,600);

    //sombra
    glShadeModel(GL_SMOOTH);

    // 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //3D
    glDisable(GL_DEPTH_TEST);

    //para uso da imagem.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //carrega imagem.
    unsigned int person_textura = 0;
    person_textura = loadTexture("sprites/nave.png");

     unsigned int powerMed_textura = 0;
    powerMed_textura = loadTexture("sprites/power_med.png");

     unsigned int powerVel_textura = 0;
    powerVel_textura = loadTexture("sprites/power_speed.png");

     unsigned int powerPlas_textura = 0;
    powerPlas_textura = loadTexture("sprites/power_plasma.png");

    unsigned int fundo_textura = 0;
    fundo_textura = loadTexture("sprites/bg.png");

    unsigned int tiro_textura = 0;
    tiro_textura = loadTexture("sprites/tiro.png");

    unsigned int inimigo_textura = 0;
    inimigo_textura = loadTexture("sprites/ovni.png");

    unsigned int inimigo2_textura = 0;
    inimigo2_textura = loadTexture("sprites/asteroide.png");

    unsigned int inimigo3_textura = 0;
    inimigo3_textura = loadTexture("sprites/asteroide2.png");

    unsigned int inimigo4_textura = 0;
    inimigo4_textura = loadTexture("sprites/asteroide3.png");

    bool executando = true;
    SDL_Event eventos;

    //loop
    while(executando)
    {
        //eventos
        while(SDL_PollEvent(&eventos))
        {
            //fecha com x da janela.
            if(eventos.type == SDL_QUIT)
            {
                executando = false;
            }
            if(estruturaHud.hudTamX <= 0){
                Mix_PlayChannel(-1, explosao, 0);
                SDL_Delay(500);
                executando = false;
            }
            //fecha com esc.
            if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE){
                Mix_PlayChannel(-1, explosao, 0);
                SDL_Delay(500);
                executando = false;
            }

            //verifica quais teclas o usuario esta apertando enquanto o programa esta em execução
            if(eventos.type == SDL_KEYDOWN){
                if(eventos.key.keysym.sym == SDLK_LEFT){
                    estruturaPlayer.esquerda = true;
                }
                else if(eventos.key.keysym.sym == SDLK_RIGHT){
                    estruturaPlayer.direita = true;
                }
                else if(eventos.key.keysym.sym == SDLK_UP){
                    estruturaPlayer.cima = true;
                }
                else if(eventos.key.keysym.sym == SDLK_DOWN){
                    estruturaPlayer.baixo = true;
                }
                else if(eventos.key.keysym.sym == SDLK_SPACE){
                    estruturaPlayer.tiro = true;
                }
            }
            else if(eventos.type == SDL_KEYUP){
                if(eventos.key.keysym.sym == SDLK_LEFT){
                    estruturaPlayer.esquerda = false;
                }
                else if(eventos.key.keysym.sym == SDLK_RIGHT){
                    estruturaPlayer.direita = false;
                }
                else if(eventos.key.keysym.sym == SDLK_UP){
                    estruturaPlayer.cima = false;
                }
                else if(eventos.key.keysym.sym == SDLK_DOWN){
                    estruturaPlayer.baixo = false;
                }
                else if(eventos.key.keysym.sym == SDLK_SPACE){
                    estruturaPlayer.tiro = false;
                }
            }

        }
    //instanciando player (Variavel "Som" do tipo int recebe o retorno da função para selecionar o som).
    Som = playerSpawn();
    if(Som == 1) Mix_PlayChannel(-1, tiro, 0);
    else if(Som == 2) Mix_PlayChannel(2, explosao, 0);

    //instanciando inimigo "Ovni"  (Variavel "Som" do tipo int recebe o retorno da função para selecionar o som).
    Som = ovniSpawn();
    if(Som == 1) Mix_PlayChannel(-1, tiro, 0);
    else if(Som == 2) Mix_PlayChannel(2, explosao, 0);

    //instanciando inimigo  (Variavel "Som" do tipo int recebe o retorno da função para selecionar o som).
    Som = inimigoSpawn(0);
    if(Som == 1) Mix_PlayChannel(-1, tiro, 0);
    else if(Som == 2) Mix_PlayChannel(2, explosao, 0);

    //instanciando inimigo  (Variavel "Som" do tipo int recebe o retorno da função para selecionar o som).
    Som = inimigoSpawn(1);
    if(Som == 1) Mix_PlayChannel(-1, tiro, 0);
    else if(Som == 2) Mix_PlayChannel(2, explosao, 0);

    //instanciando inimigo  (Variavel "Som" do tipo int recebe o retorno da função para selecionar o som).
    Som = inimigoSpawn(2);
    if(Som == 1) Mix_PlayChannel(-1, tiro, 0);
    else if(Som == 2) Mix_PlayChannel(2, explosao, 0);

    estruturaPowerUp[0].tipo = 1;
    estruturaPowerUp[1].tipo = 2;
    estruturaPowerUp[2].tipo = 3;
    Som = powerUpSpawn();
    if(Som == 1) Mix_PlayChannel(1, powerUp_med, 0);
    else if(Som == 2) Mix_PlayChannel(1, powerUp_speed, 0);
    else if(Som == 3) Mix_PlayChannel(1, powerUp_plasma, 0);

    //RENDERIZAÇÃO
    glClear(GL_COLOR_BUFFER_BIT);

    //inicia matriz
    glPushMatrix();

    //Dimensoes da matriz
    glOrtho(0, 600, 400, 0, -1, 1);

    //cor
    //glColor3d(1 ou 0);
    //glColor3f(decimais);
    //glColor3ub(r, g, b) . 0 - 255
    //glColor4ub(r, g, b, a). 0 - 155

    //fundo
    glColor4ub(255, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fundo_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(0, 0);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(600,0);//segunto ponto
    glColor4ub(0, 0, 100, 255); //branco
    glTexCoord2d(0, 0); glVertex2f(600, 400);//primeiro ponto
    glTexCoord2d(0, 1);glVertex2f(0,400);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //POWER UP med
    glColor4ub(255, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, powerMed_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaPowerUp[0].posX, estruturaPowerUp[0].posY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaPowerUp[0].posX+estruturaPowerUp[0].tam, estruturaPowerUp[0].posY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaPowerUp[0].posX+estruturaPowerUp[0].tam, estruturaPowerUp[0].posY+estruturaPowerUp[0].tam);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaPowerUp[0].posX,estruturaPowerUp[0].posY+estruturaPowerUp[0].tam);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //POWER UP velocidade
    glColor4ub(255, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, powerVel_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaPowerUp[1].posX, estruturaPowerUp[1].posY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaPowerUp[1].posX+estruturaPowerUp[1].tam, estruturaPowerUp[1].posY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaPowerUp[1].posX+estruturaPowerUp[1].tam, estruturaPowerUp[1].posY+estruturaPowerUp[1].tam);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaPowerUp[1].posX,estruturaPowerUp[1].posY+estruturaPowerUp[1].tam);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //POWER UP raioplasma
    glColor4ub(255, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, powerPlas_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaPowerUp[2].posX, estruturaPowerUp[2].posY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaPowerUp[2].posX+estruturaPowerUp[2].tam, estruturaPowerUp[2].posY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaPowerUp[2].posX+estruturaPowerUp[2].tam, estruturaPowerUp[2].posY+estruturaPowerUp[2].tam);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaPowerUp[2].posX,estruturaPowerUp[2].posY+estruturaPowerUp[2].tam);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tiro
    glColor4ub(estruturaPlayer.red, estruturaPlayer.green, estruturaPlayer.blue, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tiro_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaPlayer.balaX, estruturaPlayer.balaY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaPlayer.balaX+estruturaPlayer.balaTam, estruturaPlayer.balaY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaPlayer.balaX+estruturaPlayer.balaTam, estruturaPlayer.balaY+estruturaPlayer.balaTam);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaPlayer.balaX,estruturaPlayer.balaY+estruturaPlayer.balaTam);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //player
    glColor4ub(255, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, person_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaPlayer.personX, estruturaPlayer.personY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaPlayer.personX+estruturaPlayer.personComp, estruturaPlayer.personY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaPlayer.personX+estruturaPlayer.personComp, estruturaPlayer.personY+estruturaPlayer.personAlt);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaPlayer.personX, estruturaPlayer.personY+estruturaPlayer.personAlt);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //inimigo
    glColor4ub(255, 255, 255, 255); //vermelho
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, inimigo2_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaInimigo[0].inimigoX, estruturaInimigo[0].inimigoY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaInimigo[0].inimigoX+estruturaInimigo[0].inimigoCA, estruturaInimigo[0].inimigoY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaInimigo[0].inimigoX+estruturaInimigo[0].inimigoCA, estruturaInimigo[0].inimigoY+estruturaInimigo[0].inimigoCA);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaInimigo[0].inimigoX, estruturaInimigo[0].inimigoY+estruturaInimigo[0].inimigoCA);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //inimigo
    glColor4ub(255, 255, 255, 255); //vermelho
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, inimigo3_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaInimigo[1].inimigoX, estruturaInimigo[1].inimigoY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaInimigo[1].inimigoX+estruturaInimigo[1].inimigoCA, estruturaInimigo[1].inimigoY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaInimigo[1].inimigoX+estruturaInimigo[1].inimigoCA, estruturaInimigo[1].inimigoY+estruturaInimigo[1].inimigoCA);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaInimigo[1].inimigoX, estruturaInimigo[1].inimigoY+estruturaInimigo[1].inimigoCA);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //inimigo
    glColor4ub(255, 255, 255, 255); //vermelho
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, inimigo4_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaInimigo[2].inimigoX, estruturaInimigo[2].inimigoY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaInimigo[2].inimigoX+estruturaInimigo[2].inimigoCA, estruturaInimigo[2].inimigoY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaInimigo[2].inimigoX+estruturaInimigo[2].inimigoCA, estruturaInimigo[2].inimigoY+estruturaInimigo[2].inimigoCA);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaInimigo[2].inimigoX, estruturaInimigo[2].inimigoY+estruturaInimigo[2].inimigoCA);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tiro ovni
    glColor4ub(0, 255, 255, 255); //branco
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tiro_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaOvni.balaX, estruturaOvni.balaY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaOvni.balaX+estruturaOvni.balaTam, estruturaOvni.balaY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaOvni.balaX+estruturaOvni.balaTam, estruturaOvni.balaY+estruturaOvni.balaTam);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaOvni.balaX,estruturaOvni.balaY+estruturaOvni.balaTam);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //ovni
    glColor4ub(255, 255, 255, 255); //vermelho
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, inimigo_textura);
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaOvni.inimigoX, estruturaOvni.inimigoY);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaOvni.inimigoX+estruturaOvni.inimigoCA, estruturaOvni.inimigoY);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaOvni.inimigoX+estruturaOvni.inimigoCA, estruturaOvni.inimigoY+estruturaOvni.inimigoCA);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaOvni.inimigoX, estruturaOvni.inimigoY+estruturaOvni.inimigoCA);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //HUD
    glColor4ub(255, 50, 50, 255); //vermelho
    glBegin(GL_QUADS); //GL_POINT, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
    glTexCoord2d(0, 0); glVertex2f(estruturaHud.xHud, estruturaHud.yHud);//primeiro ponto
    glTexCoord2d(1, 0); glVertex2f(estruturaHud.xHud+estruturaHud.hudTamX, estruturaHud.yHud);//segunto ponto
    glTexCoord2d(1, 1); glVertex2f(estruturaHud.xHud+estruturaHud.hudTamX, estruturaHud.yHud+estruturaHud.hudTamY);//primeiro ponto
    glTexCoord2d(0, 1); glVertex2f(estruturaHud.xHud,estruturaHud.yHud+estruturaHud.hudTamY);//segunto ponto
    //fecha
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //fecha matriz
    glPopMatrix();

    //ANIMAÇÃO
    SDL_GL_SwapBuffers();
    }
SDL_FreeSurface(icone);
SDL_FreeSurface(tela);
Mix_FreeMusic(musica);
Mix_FreeChunk(tiro);
Mix_FreeChunk(explosao);
Mix_FreeChunk(powerUp_med);
Mix_FreeChunk(powerUp_plasma);
Mix_FreeChunk(powerUp_speed);
Mix_CloseAudio();
glDisable(GL_BLEND);
SDL_Quit();
fprintf(fp,"%2.f\n", estruturaPlayer.pontos);
}
