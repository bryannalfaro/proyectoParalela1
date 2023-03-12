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

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }
    // initial position of the rectangles
    // Set initial ball position and velocity
    float posX = windowWidth / 2;
    float posY = windowHeight / 2;
    float velX = 100;
    float velY = 0;

    // Set ball radius
    float radius = 20;

    // Set angle of circular movement
    float angle = 0;
    float angularSpeed = 1;  // radians per second

    // Game loop
    Uint32 prevTicks = SDL_GetTicks();
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        // Calculate delta time
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - prevTicks) / 1000.0f;
        prevTicks = currentTicks;

        // Update angle
        angle += angularSpeed * deltaTime;

        // Calculate new ball position
        posX = windowWidth / 2 + radius * cos(angle);
        posY = windowHeight / 2 + radius * sin(angle);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect ballRect = { (int)(posX - radius), (int)(posY - radius), (int)(2 * radius), (int)(2 * radius) };
        SDL_RenderFillRect(renderer, &ballRect);

        // Swap buffers
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}