//TODO
//cambiar amplitudes y periodos
//choques cambian movimiento y particiona
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>



int main(int argc, char *argv[])
{
    int windowWidth = 640;
    int windowHeight = 480;

    int phi = 10;
    int N = 10;
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
    int initial_pos[N][2];
    int amplitudes[N][2];
    int directions[N];
    int periods[N];
    int colors[N][3];
    int angular_velocities[N];
     srand(time(NULL));
    //set the properties of the rectangles
    for (int i = 0; i < N; i++) {
        rect[i].x = rand() % windowWidth;
        rect[i].y = rand() % windowWidth;
        int x =rand() % 50;
        rect[i].w = x;
        rect[i].h = x;
        initial_pos[i][0] = rect[i].x;
        initial_pos[i][1] = rect[i].y;
        directions[i] = 1;

        amplitudes[i][0] = rand() % 100;
        amplitudes[i][1] = amplitudes[i][0];
        periods[i] = rand() % 10;
        //asign random colors to the rectangles;
        colors[i][0] = rand() % 255;
        colors[i][1] = rand() % 255;
        colors[i][2] = rand() % 255;

        angular_velocities[i] = ((2*M_PI)/periods[i]);

    }

    double period = 2; // The period of the cosine wave


    //create color for the rectangles

    for (int i = 0; i < N; i++) {
        SDL_SetRenderDrawColor(rend, colors[i][0], colors[i][1], colors[i][2], 255);
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
            //Check collision with the others
            for (int i = 0; i < N; i++) {
                if (i == j) {
                    continue;
                }
               SDL_bool inter = SDL_HasIntersection(&rect[j], &rect[i]);
               if(inter) {
                    // already_collidedX = i;
                    // already_collidedY = j;
                    printf("collision detected en i: %d, j: %d\n", i, j);
                    //change color of the rectangles

                    // colors[j][0] = rand() % 255;
                    // colors[j][1] = rand() % 255;
                    // colors[j][2] = rand() % 255;
                    // colors[i][0] = rand() % 255;
                    // colors[i][1] = rand() % 255;
                    // colors[i][2] = rand() % 255;

                    //change the initial position of the rectangles
                    // rect[j].x = rand() % windowWidth;
                    // rect[j].y = rand() % windowWidth;
                    // rect[i].x = rand() % windowWidth;
                    // rect[i].y = rand() % windowWidth;
                    // change the direction of the squares movement
                    angular_velocities[j] = -angular_velocities[j];
                    angular_velocities[i] = -angular_velocities[i];
                    rect[i].x = initial_pos[i][0] + amplitudes[i][0] * cos(angular_velocities[i] * time_e+phi);
                    rect[i].y = initial_pos[i][1] + amplitudes[i][1] * sin(angular_velocities[i] * time_e+phi);
               }

            }
            //use cosine function to move the rectangles
            rect[j].x = initial_pos[j][0] + amplitudes[j][0] * cos(angular_velocities[j] * time_e+phi);
            rect[j].y = initial_pos[j][1] + amplitudes[j][1] * sin(angular_velocities[j] * time_e+phi);
            //SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            SDL_SetRenderDrawColor(rend, colors[j][0], colors[j][1], colors[j][2], 255);
            SDL_RenderFillRect(rend, &rect[j]);

        }
        SDL_RenderPresent(rend);
        time_e += 0.01; // Increase the time elapsed

        SDL_Delay(100); // Wait for a few milliseconds to slow down the animation


    }

    //destroy all
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}