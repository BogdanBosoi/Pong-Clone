#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_TTF.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cmath>
///#include "script.h"

#define PI 3.141259265

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 800;
int scoreplayer1, scoreplayer2;
class wallTile
{
public:
    SDL_Texture *tex;
    SDL_Surface *img;
    SDL_Rect obj;
    char texture[50];
    void set_values(char name[], int posx, int posy, int wid, int hei, SDL_Window* window, SDL_Renderer* renderer)
    {
        strcpy(texture, name);
        img = IMG_Load(texture);
        obj.x = posx;
        obj.y = posy;
        obj.w = wid;
        obj.h = hei;
        tex = SDL_CreateTextureFromSurface(renderer, img);
    }
};

int check_collision(SDL_Rect A, SDL_Rect B)
{
    int leftB, leftA, rightA, rightB, topA, topB, bottomA, bottomB;

    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    if(bottomA <= topB)
        return false;
    if(topA >= bottomB)
        return false;
    if(rightA <= leftB)
        return false;
    if(leftA >= rightB)
        return false;

    if(!(bottomA <= topB && topA >= bottomB))
        return 2;

    return true;
}

class textObj
{
public:
    SDL_Texture *tex;
    SDL_Color White = {255, 255, 255};
    SDL_Surface *img;
    char text[50];
    SDL_Rect obj;
    void set_values(char name[], int posx, int posy, int wid, int hei, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font)
    {
        strcpy(text, name);
        obj.x = posx;
        obj.y = posy;
        obj.w = wid;
        obj.h = hei;
        img = TTF_RenderText_Solid(font, text, White);
        tex = SDL_CreateTextureFromSurface(renderer, img);

    }
};


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Pong",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH,
                                          HEIGHT,
                                          SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;
    TTF_Font *font;
    font = TTF_OpenFont("font.ttf", 32);

    textObj AIButton, playerButton, exitButton, titleButton;
    titleButton.set_values("PONG", WIDTH/2 - 64, 128, 100, 100, window, renderer, font);
    AIButton.set_values("Play vs AI", WIDTH/2 - 100, 400, 200, 100, window, renderer, font);
    playerButton.set_values("Play vs Player", WIDTH/2 - 150, 500, 300, 100, window, renderer, font);
    exitButton.set_values("Exit", WIDTH/2 - 64, 600, 100, 100, window, renderer, font);
reset:
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    bool vsPlayer = 0;
    bool quit = 0;
    int mouse_x, mouse_y;
    wallTile fBlocks[WIDTH/32 + 1], sBlocks[WIDTH/32 + 1];
    wallTile center[HEIGHT/64 + 1];
    wallTile player;
    wallTile ball;
    wallTile AI;
    textObj winGuy;
    SDL_Color White = {255, 255, 255};
    SDL_Rect score1 = {WIDTH/2 - 192, HEIGHT/2 - 32, 100, 100};
    SDL_Rect score2 = {WIDTH/2 + 128, HEIGHT/2 - 32, 100, 100};
    for(int i = 0; i <= WIDTH/32; i++)
    {
        fBlocks[i].set_values("images\\wall.png", i * 32, 0, 32, 32, window, renderer);
        sBlocks[i].set_values("images\\wall.png", i * 32, HEIGHT - 32, 32, 32, window, renderer);
    }
    for(int i = 0; i <= HEIGHT/64; i++)
        center[i].set_values("images\\wall.png", WIDTH/2, i * 64, 32, 32, window, renderer);
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(1);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if(mouse_x >= AIButton.obj.x && mouse_x <= AIButton.obj.x + AIButton.obj.w && mouse_y >= AIButton.obj.y && mouse_y <= AIButton.obj.y + AIButton.obj.h)
                    vsPlayer = 0, quit = 1;
                if(mouse_x >= playerButton.obj.x && mouse_x <= playerButton.obj.x + playerButton.obj.w && mouse_y >= playerButton.obj.y && mouse_y <= playerButton.obj.y + playerButton.obj.h)
                    vsPlayer = 1, quit = 1;
                if(mouse_x >= exitButton.obj.x && mouse_x <= exitButton.obj.x + exitButton.obj.w && mouse_y >= exitButton.obj.y && mouse_y <= exitButton.obj.y +exitButton.obj.h)
                    exit(1);
                break;
            }
        }
        SDL_RenderCopy(renderer, titleButton.tex, NULL, &titleButton.obj);
        SDL_RenderCopy(renderer, AIButton.tex, NULL, &AIButton.obj);
        SDL_RenderCopy(renderer, playerButton.tex, NULL, &playerButton.obj);
        SDL_RenderCopy(renderer, exitButton.tex, NULL, &exitButton.obj);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

restart:
    player.set_values("images\\bat.png", 32, HEIGHT/2, 32, 128, window, renderer);
    ball.set_values("images\\ball.png", WIDTH/2, HEIGHT/2, 32, 32, window, renderer);
    player.obj.y -= player.obj.h/2;
    AI.set_values("images\\bat.png", WIDTH - 64, HEIGHT/2, 32, 128, window, renderer);


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for(int i = 0; i <= WIDTH/32; i++)
    {
        SDL_RenderCopy(renderer, fBlocks[i].tex, NULL, &fBlocks[i].obj);
        SDL_RenderCopy(renderer, sBlocks[i].tex, NULL, &sBlocks[i].obj);
    }
    for(int i = 0; i <= HEIGHT/64; i++)
        SDL_RenderCopy(renderer, center[i].tex, NULL, &center[i].obj);
    SDL_RenderPresent(renderer);
    quit = 0;
    int N;
    srand(time(NULL));
    do
    {
        N = rand() % 360;
    }
    while ((N >= 20 && N <= 70) || (N >= 110 && N <= 160) || (N >= 200 && N <= 250) || (N >= 290 && N <= 350));
    float relx = sin(N * PI/180);
    float rely = cos(N * PI/180);
    if(rely == 0)
        rely = (rand() % 100) / 100;
    if(relx == 0)
        relx = (rand() % 100) / 100;
    int xspeed = rand() % 10 + 4;
    int yspeed = rand() % 10 + 4;
    bool verifdelay = 0;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
        char tScore1[10];
        itoa(scoreplayer1, tScore1, 10);
        char tScore2[10];
        itoa(scoreplayer2, tScore2, 10);
        SDL_Surface* surfaceScore1 = TTF_RenderText_Solid(font, tScore1, White);
        SDL_Surface* surfaceScore2 = TTF_RenderText_Solid(font, tScore2, White);
        SDL_Texture* texScore1 = SDL_CreateTextureFromSurface(renderer, surfaceScore1);
        SDL_Texture* texScore2 = SDL_CreateTextureFromSurface(renderer, surfaceScore2);
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
        if(currentKeyStates[SDL_SCANCODE_W])
            if(player.obj.y >= fBlocks[1].obj.y + 31 && player.obj.y <= sBlocks[1].obj.y - 127)
                player.obj.y -= 6;
        if(currentKeyStates[SDL_SCANCODE_S])
            if(player.obj.y >= fBlocks[1].obj.y + 31 && player.obj.y <= sBlocks[1].obj.y - 127)
                player.obj.y += 6;
        if(currentKeyStates[SDL_SCANCODE_ESCAPE])
        {
            scoreplayer1 = 0;
            scoreplayer2 = 0;
            goto reset;
        }
        if(player.obj.y <= fBlocks[1].obj.y + 32)
            player.obj.y = 32;
        if(player.obj.y >= sBlocks[1].obj.y - 127)
            player.obj.y = HEIGHT - 160;
        if (vsPlayer == 1)
        {
            if(currentKeyStates[SDL_SCANCODE_I])
                if(AI.obj.y >= fBlocks[1].obj.y + 31 && AI.obj.y <= sBlocks[1].obj.y - 127)
                    AI.obj.y -= 6;
            if(currentKeyStates[SDL_SCANCODE_K])
                if(AI.obj.y >= fBlocks[1].obj.y + 31 && AI.obj.y <= sBlocks[1].obj.y - 127)
                    AI.obj.y += 6;
            if(AI.obj.y <= fBlocks[1].obj.y + 32)
                AI.obj.y = 64;
            if(AI.obj.y >= sBlocks[1].obj.y - 127)
                AI.obj.y = HEIGHT - 160;

        }




//        cout << persx << " "<< persy << "\n";
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        ball.obj.x += xspeed * relx;
        ball.obj.y += yspeed * rely;
        /*if (ball.obj.y + yspeed * rely <= 32 && verifdelay == 0)
            rely *= -1, verifdelay = 1;
        else
            verifdelay = 0;*/
        if(ball.obj.x > 31 && ball.obj.x + 32 < WIDTH - 31)
        {
            if (ball.obj.y <= 32)
                rely *= -1, ball.obj.y = 33;
            if(ball.obj.y + 32>= HEIGHT - 33)
                rely *= -1;
            if(check_collision(ball.obj, player.obj) == 2)
                relx *= -1, xspeed = rand() % 10 + 4, yspeed = rand() % 10 + 4, ball.obj.x = player.obj.x + 33;
            if(check_collision(ball.obj, AI.obj) == 2)
                relx *= -1, xspeed = rand() % 10 + 4, yspeed = rand() % 10 + 4, ball.obj.x = AI.obj.x - 33;
            /**if(ball.obj.x + xspeed * relx <= player.obj.x + 32 && (ball.obj.y + yspeed * rely >= player.obj.y - 31 && ball.obj.y + yspeed * rely <= player.obj.y + 128))
                relx *= -1, xspeed = rand() % 10 + 4, yspeed = rand() % 10 + 4, ball.obj.x = player.obj.x + 33;
            if(ball.obj.x + 32 >= AI.obj.x && (ball.obj.y - 31 >= AI.obj.y && ball.obj.y <= AI.obj.y + 128))
                relx *= -1, xspeed = rand() % 10 + 4, yspeed = rand() % 10 + 4, ball.obj.x = AI.obj.x - 33;
            /*if(ball.obj.x <= player.obj.x + 32 && ball.obj.y >= player.obj.y && ball.obj.y <= player.obj.y + 128)
                rely *= -1;
            if(ball.obj.x <= player.obj.x + 32 && ball.obj.y <= player.obj.y + 128 && ball.obj.y >= player.obj.y)
                rely *= -1;
            if(ball.obj.x + 32 >= AI.obj.x && ball.obj.y >= AI.obj.y && ball.obj.y <= AI.obj.y + 128)
                rely *= -1;
            if(ball.obj.x + 32 >= AI.obj.x && ball.obj.y <= AI.obj.y + 128 && ball.obj.y >= AI.obj.y)
                rely *= -1;*/
        }
        if(vsPlayer == 0)
        {
            if(AI.obj.y + 48 < ball.obj.y)
                AI.obj.y += 6;
            else if(AI.obj.y + 48 > ball.obj.y)
                AI.obj.y -= 6;
        }
        if(ball.obj.x <= 0)
        {
            scoreplayer2++;
            goto restart;
        }
        if(ball.obj.x >= WIDTH)
        {
            scoreplayer1++;
            goto restart;
        }



        for(int i = 0; i <= WIDTH/32; i++)
        {
            SDL_RenderCopy(renderer, fBlocks[i].tex, NULL, &fBlocks[i].obj);
            SDL_RenderCopy(renderer, sBlocks[i].tex, NULL, &sBlocks[i].obj);
        }
        for(int i = 0; i <= HEIGHT/64; i++)
            SDL_RenderCopy(renderer, center[i].tex, NULL, &center[i].obj);

        SDL_RenderCopy(renderer, player.tex, NULL, &player.obj);
        SDL_RenderCopy(renderer, AI.tex, NULL, &AI.obj);
        SDL_RenderCopy(renderer, ball.tex, NULL, &ball.obj);
        SDL_RenderCopy(renderer, texScore1, NULL, &score1);
        SDL_RenderCopy(renderer, texScore2, NULL, &score2);
        SDL_RenderPresent(renderer);

        if(scoreplayer1 == 7)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            winGuy.set_values("Player 1 Wins!", WIDTH/2 - 250, HEIGHT/2 - 150, 500, 200, window, renderer, font);
            SDL_RenderCopy(renderer, winGuy.tex, NULL, &winGuy.obj);
            SDL_Delay(2000);
            scoreplayer1 = 0;
            scoreplayer2 = 0;
            goto reset;
        }
        else if(scoreplayer2 == 7)
        {
            if(vsPlayer == 0)
                winGuy.set_values("AI Wins!", WIDTH/2 - 90, HEIGHT/2, 200, 200, window, renderer, font);
            else
                winGuy.set_values("Player 2 Wins!", WIDTH/2 - 250, HEIGHT/2 - 150, 500, 200, window, renderer, font);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, winGuy.tex, NULL, &winGuy.obj);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            scoreplayer1 = 0;
            scoreplayer2 = 0;
            goto reset;
        }

        SDL_Delay(10);
    }

    return 0;
}
