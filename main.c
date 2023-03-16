//TODO
// que no nazca montado
// revisar que no este muy cerca del edge
// que no detectar que los choques no se hagan dos veces
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>



int main(int argc, char *argv[])
{
    int N = 30;
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    int windowWidth = 640;
    int windowHeight = 480;
    int frames = 0;

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME COSINE", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       windowWidth, windowHeight, 0);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }
    // initial position of the rectangles
    //create an array of N  rectangles
    SDL_Rect rect[N];
    // Set initial ball position and velocity
    int positions[N][4];
    float angularSpeeds[N];
    float angle[N];
    int radius[N];
    int colors[N][3];

    for (int i = 0; i < N; i++) {
        positions[i][0] = (rand() %(windowWidth - 45 + 1)) + 45;
        positions[i][1] = (rand() %(windowHeight - 45 + 1)) + 45;
        rect[i].x = positions[i][0];
        rect[i].y = positions[i][1];
        int size = rand() % 50 + 10;
        rect[i].w = size;
        rect[i].h = size;
        positions[i][2] = positions[i][0];
        positions[i][3] = positions[i][1];
        colors[i][0] = rand() % 255;
        colors[i][1] = rand() % 255;
        colors[i][2] = rand() % 255;
        // set initial angular speed randomly as 1 or -1
        int x = rand() % 2;
        if (x == 0) {
            angularSpeeds[i] = -1;
        } else {
            angularSpeeds[i] = 1;
        }
        angle[i] = rand() % 360;
        radius[i] = rand() % N + size;
    }

    // Game loop
    Uint32 prevTicks = SDL_GetTicks();
    Uint32 start = SDL_GetTicks();
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Calculate delta time
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - prevTicks) / 1000.0f;
        prevTicks = currentTicks;
    
        for (int i = 0; i < N; i++) {
            // check for collision with the walls
            if (rect[i].x < 0 || rect[i].x > windowWidth - rect[i].w) {
                angularSpeeds[i] *= -1;
            }
            if (rect[i].y < 0 || rect[i].y > windowHeight - rect[i].h) {
                angularSpeeds[i] *= -1;
            }
            for (int j = 0; j < N; j++) {
                // Check for collision with other balls and swap velocities if collision occurs
                if (i != j) {
                    SDL_bool inter = SDL_HasIntersection(&rect[j], &rect[i]);
                    if (inter) {
                        angularSpeeds[i] *= -1;
                        angularSpeeds[j] *= -1;
                    }
                }
                
            }
            // Update angle
            angle[i] += angularSpeeds[i] * deltaTime;

            rect[i].x = positions[i][2] + radius[i] * cos(angle[i]);
            rect[i].y = positions[i][3] + radius[i] * sin(angle[i]);
            // Draw ball
            SDL_SetRenderDrawColor(renderer, colors[i][0], colors[i][1], colors[i][2], 255);
            SDL_RenderFillRect(renderer, &rect[i]);

        }
        // Swap buffers
        SDL_RenderPresent(renderer);

        frames++;
        Uint32 end = SDL_GetTicks();
        Uint32 elapsed = end - start;

        if (elapsed >= 1000) {
            double fps = frames / (elapsed / 1000.0);
            printf("FPS: %f\n", fps);
            frames = 0; 
            start = end;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
