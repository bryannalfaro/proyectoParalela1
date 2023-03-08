#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>



int main(int argc, char *argv[])
{
    int windowWidth = 640;
    int windowHeight = 480;
    int amplitud = 100;
    int phi = 10;
    int N = 25;
    bool game_is_running = true;

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME COSINE", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       windowWidth, windowHeight, 0);


   //render N rectangles
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //create an array of N  rectangles
    SDL_Rect rect[N];
    // initial position of the rectangles
    int initial_pos[N];

    //set the properties of the rectangles
    for (int i = 0; i < N; i++) {
        rect[i].x = rand() % windowWidth;
        rect[i].y = rand() % windowWidth;
        rect[i].w = 10;
        rect[i].h = 10;
        initial_pos[i] = rect[i].x;
    }

    double period = 2; // The period of the cosine wave


    //create color for the rectangles
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < N; i++) {
        SDL_RenderFillRect(rend, &rect[i]);
        SDL_RenderPresent(rend);
    }
    srand(time(NULL));
    double time_e = 0; // Time elapsed
    while (game_is_running) {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            game_is_running = false;
            break;
        }

        //clear the screen
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        for (int j = 0; j < N; j++) {
            //use cosine function to move the rectangles
            rect[j].x = initial_pos[j] + amplitud * cos(((2*M_PI)/period)*time_e+phi);
            SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_RenderFillRect(rend, &rect[j]);

        }
        SDL_RenderPresent(rend);
        time_e += 0.01; // Increase the time elapsed

        SDL_Delay(10); // Wait for a few milliseconds to slow down the animation


    }

    //destroy all
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}