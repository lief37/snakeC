#include "SDL2/SDL.h" 
#include <stdio.h>
#include <vector>
using namespace std; 

const int WINDOW_SIDE = 400;
const int NB_PIXELS = 20;
const int PIXEL_SIDE = WINDOW_SIDE / NB_PIXELS;

const int UP_DIRECTION = 1;
const int DOWN_DIRECTION = 2;
const int RIGHT_DIRECTION = 3;
const int LEFT_DIRECTION = 4;

vector<int> snakex{NB_PIXELS/2 - 1};
vector<int> snakey{NB_PIXELS/2 - 1};

int direction = 0;

int fruitX;
int fruitY;

void draw(SDL_Renderer* renderer);
void move();
void resetBonus();
bool hitsSnake();

int main( int argc, char* args[] ) 
{ 
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Snake", 100, 100, WINDOW_SIDE, WINDOW_SIDE, 0);
   
    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    bool alive = 1;

    resetBonus();
    
    while(alive) {
        draw(renderer);

        SDL_Event event;
        /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
        /* more events on the event queue, our while loop will exit when */
        /* that occurs.                                                  */
        while( SDL_PollEvent( &event ) ){
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            if (event.type == SDL_KEYDOWN){
                switch( event.key.keysym.sym ){
                    case SDLK_UP:
                        direction = UP_DIRECTION;
                        break;

                    case SDLK_DOWN:
                        direction = DOWN_DIRECTION;
                        break;

                    case SDLK_LEFT:
                        direction = LEFT_DIRECTION;
                        break;

                    case SDLK_RIGHT:
                        direction = RIGHT_DIRECTION;
                        break;

                    case SDLK_ESCAPE:
                        alive=0;
                        break;

                    default:
                        break;
                }
            }       
        }


        move();
        SDL_Delay(100);
    }

    SDL_DestroyWindow(window);

    SDL_Quit(); 

    return 0;   
}

void draw(SDL_Renderer* renderer) {
    // Set render color to white ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

    // Clear window
    SDL_RenderClear( renderer );

    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect rFruit;
    rFruit.x = PIXEL_SIDE*fruitX;
    rFruit.y = PIXEL_SIDE*fruitY;
    rFruit.w = PIXEL_SIDE - 1;
    rFruit.h = PIXEL_SIDE - 1;

    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    // Render rect
    SDL_RenderFillRect( renderer, &rFruit );

    for(int i=0; i <snakex.size(); i++){
        // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
        SDL_Rect r;
        r.x = PIXEL_SIDE*snakex.at(i);
        r.y = PIXEL_SIDE*snakey.at(i);
        r.w = PIXEL_SIDE - 1;
        r.h = PIXEL_SIDE - 1;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

        // Render rect
        SDL_RenderFillRect( renderer, &r );
    }
    
    // Render the rect to the screen
    SDL_RenderPresent(renderer);
}

void move() {
    int lastSnakex = snakex.back();
    int lastSnakey = snakey.back();

    for(int i=snakex.size() -1; i>0; i--) {
        snakex.at(i) = snakex.at(i-1);
        snakey.at(i) = snakey.at(i-1);
    }


    switch (direction) {
        case UP_DIRECTION:
            if(snakey.at(0) == 0) {
                snakey.at(0) = NB_PIXELS - 1;
            } else {
                snakey.at(0) -= 1;
            }
            break;
        case DOWN_DIRECTION:
            if(snakey.at(0) == NB_PIXELS - 1) {
                snakey.at(0) = 0;
            } else {
                snakey.at(0) += 1;
            }
            break;
        case RIGHT_DIRECTION:
            if(snakex.at(0) == NB_PIXELS - 1) {
                snakex.at(0) = 0;
            } else {
                snakex.at(0) += 1;
            }
            break;
        case LEFT_DIRECTION:
            if(snakex.at(0) == 0) {
                snakex.at(0) = NB_PIXELS - 1;
            } else {
                snakex.at(0) -= 1;
            }
            break;
        default:
            break;
    }

    if (fruitX == snakex.at(0) && fruitY == snakey.at(0)){
        snakex.push_back(lastSnakex);
        snakey.push_back(lastSnakey);
        resetBonus();
    }

    if (hitsSnake()) {
        snakex = {NB_PIXELS/2 - 1};
        snakey = {NB_PIXELS/2 - 1};
        direction = 0;
        resetBonus();
    }
}

void resetBonus() {
    bool unvalidFruit = 1;

    while (unvalidFruit){
        fruitX = rand() % NB_PIXELS;
        fruitY = rand() % NB_PIXELS;

        unvalidFruit = 0;
        for (int i=0; i< snakex.size(); i++){
            if (fruitX == snakex.at(i) && fruitY == snakey.at(i)){
                unvalidFruit = 1;
                break;
            }
        }
    }
}

bool hitsSnake() {
    bool hitsSnake = 0;
    for (int i=1; i< snakex.size(); i++){
        if (snakex.at(0) == snakex.at(i) && snakey.at(0) == snakey.at(i)){
            hitsSnake = 1;
            break;
        }
    }
    return hitsSnake;
}