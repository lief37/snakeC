#include "SDL2/SDL.h" 
#include <stdio.h>
#include <vector>
using namespace std; 

const int WINDOW_SIDE = 400;
const int NB_PIXELS = 20;
const int PIXEL_SIDE = WINDOW_SIDE / NB_PIXELS;

enum Direction {NONE, UP, DOWN, RIGHT, LEFT};

class Pixel {
    public:
        int x;
        int y;
        Pixel(int xCoord, int yCoord){
            x = xCoord;
            y = yCoord;
        }
};

class Snake {
    public:
        vector<Pixel> coords;
        Direction direction;
        void resetSnake(){
            coords = {Pixel(NB_PIXELS/2 - 1, NB_PIXELS/2 - 1)};
            direction = NONE;
        };
        void move(Pixel nextPixel){
            for(int i=coords.size() -1; i>0; i--) {
                coords.at(i).x = coords.at(i-1).x;
                coords.at(i).y = coords.at(i-1).y;
            }
            coords.at(0) = nextPixel;
        }
};

class Fruit {
    public:
        int x;
        int y; 
        void resetFruit(Snake snake) {
            bool unvalidFruit = 1;

            while (unvalidFruit){
                x = rand() % NB_PIXELS;
                y = rand() % NB_PIXELS;

                unvalidFruit = 0;
                for (int i=0; i< snake.coords.size(); i++){
                    if (x == snake.coords.at(i).x && y == snake.coords.at(i).y){
                        unvalidFruit = 1;
                        break;
                    }
                }
            }
        }
};

class Controller {
    public:
        Snake snake;
        Fruit fruit;
        void initGame() {
            snake.resetSnake();
            fruit.resetFruit(snake);
        };
        void changeSnakeDirection(Direction direction) {
            snake.direction = direction;
        };
        void moveSnake(){
            // Compute the next pixel
            Pixel nextPixel = Pixel(snake.coords.at(0).x, snake.coords.at(0).y);
            switch (snake.direction) {    
                case UP:
                    if(snake.coords.at(0).y == 0) {
                        nextPixel.y = NB_PIXELS - 1;
                    } else {
                        nextPixel.y -= 1;
                    }
                    break;
                case DOWN:
                    if(snake.coords.at(0).y == NB_PIXELS - 1) {
                        nextPixel.y = 0;
                    } else {
                        nextPixel.y += 1;
                    }
                    break;
                case RIGHT:
                    if(snake.coords.at(0).x == NB_PIXELS - 1) {
                        nextPixel.x = 0;
                    } else {
                        nextPixel.x += 1;
                    }
                    break;
                case LEFT:
                    if(snake.coords.at(0).x == 0) {
                        nextPixel.x = NB_PIXELS - 1;
                    } else {
                        nextPixel.x -= 1;
                    }
                    break;
                default:
                    break;
            }

            // See if crash
            bool hitsSnake = 0;
            for (int i=0; i< snake.coords.size()-1; i++){
                if (snake.coords.at(i).x == nextPixel.x && snake.coords.at(i).y == nextPixel.y){
                    hitsSnake = 1;
                    break;
                }
            }

            // See if grow
            if (fruit.x == nextPixel.x && fruit.y == nextPixel.y){
                Pixel lastPixel = Pixel(snake.coords.at(snake.coords.size()-1).x, snake.coords.at(snake.coords.size()-1).y);
                snake.move(nextPixel);
                snake.coords.push_back(lastPixel);
                fruit.resetFruit(snake);
            } else {
                snake.move(nextPixel);
            }

            if (hitsSnake) {
                initGame();
            }

        }
};

class Renderer {
    public:
        // Setup renderer
        SDL_Renderer* renderer = NULL;
        Renderer(SDL_Window *window) {
            renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        }
        void draw(Snake snake, Fruit fruit){
             // Set render color to white ( background will be rendered in this color )
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

                // Clear window
                SDL_RenderClear( renderer );

                // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
                SDL_Rect rFruit;
                rFruit.x = PIXEL_SIDE*fruit.x;
                rFruit.y = PIXEL_SIDE*fruit.y;
                rFruit.w = PIXEL_SIDE - 1;
                rFruit.h = PIXEL_SIDE - 1;

                // Set render color to blue ( rect will be rendered in this color )
                SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

                // Render rect
                SDL_RenderFillRect( renderer, &rFruit );

                for(int i=0; i <snake.coords.size(); i++){
                    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
                    SDL_Rect r;
                    r.x = PIXEL_SIDE*snake.coords.at(i).x;
                    r.y = PIXEL_SIDE*snake.coords.at(i).y;
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
};

class Window {
    public:
        SDL_Window *window;
        Renderer renderer = NULL;
        Controller controller;
        Window() {
            SDL_Init(SDL_INIT_VIDEO);
            window = SDL_CreateWindow("Snake", 100, 100, WINDOW_SIDE, WINDOW_SIDE, 0);
            renderer = Renderer(window);
        };
        void startGame() {
            bool alive = 1;
            controller.initGame();
            while(alive) {
                renderer.draw(controller.snake, controller.fruit);
                SDL_Event event;
                /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
                /* more events on the event queue, our while loop will exit when */
                /* that occurs.                                                  */
                while( SDL_PollEvent( &event ) ){
                    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
                    if (event.type == SDL_KEYDOWN){
                        switch( event.key.keysym.sym ){
                            case SDLK_UP:
                                controller.changeSnakeDirection(UP);
                                break;

                            case SDLK_DOWN:
                                controller.changeSnakeDirection(DOWN);
                                break;

                            case SDLK_LEFT:
                                controller.changeSnakeDirection(LEFT);
                                break;

                            case SDLK_RIGHT:
                                controller.changeSnakeDirection(RIGHT);
                                break;

                            case SDLK_ESCAPE:
                                alive=0;
                                break;

                            default:
                                break;
                        }
                    }       
                }
                controller.moveSnake();
                SDL_Delay(100);
            };
        };
};

int main( int argc, char* args[] ) 
{ 
    Window window;
    window.startGame();
    return 0;   
}
