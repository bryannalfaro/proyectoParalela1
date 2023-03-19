// TODO
//  que no nazca montado
//  revisar que no este muy cerca del edge
//  que no detectar que los choques no se hagan dos veces
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include<iostream>
// #include "square.h"

#define triesNum 10
#define maxRadius 20

struct Square
{
    int x;            // x-coordinate of top left corner
    int y;            // y-coordinate of top left corner
    int initialX;     // Initial pos X
    int initialY;     // Initial pos Y
    int width;        // width of square
    int height;       // height of square
    float angle;        // Angle
    int radius;       // Radius
    float angularSpeed; // Angular speed
    SDL_Color color;  // Color
};

// Generate a random square within the bounds of the window
Square generate_random_square(SDL_Window *window)
{
    Square square;
    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);
    square.x = rand() % (winWidth - 50);  // Subtract 50 to ensure the square fits within the window
    square.y = rand() % (winHeight - 50); // Subtract 50 to ensure the square fits within the window
    square.initialX = square.x;
    square.initialY = square.y;
    // square.width = rand() % (winWidth - square.x);
    square.angle = rand() % 360;
    // set initial angular speed randomly as 1 or -1
    int x = rand() % 2;
    if (x == 0)
    {
        square.angularSpeed = -2;
    }
    else
    {
        square.angularSpeed = 2;
    }
    square.radius = rand() % maxRadius;
    square.width = (rand() % 45) + 5;
    square.height = square.width;
    square.color = {static_cast<unsigned char>(rand() % 256),
                    static_cast<unsigned char>(rand() % 256),
                    static_cast<unsigned char>(rand() % 256),
                    255}; // Generate a random color with full alpha
    return square;
}

// Check boxes colliding
bool check_collision(Square s1, Square s2)
{
    // Calculate the x- and y-coordinates of the bounding boxes
    int s1_x1 = s1.x;
    int s1_x2 = s1.x + s1.width;
    int s1_y1 = s1.y;
    int s1_y2 = s1.y + s1.height;
    int s2_x1 = s2.x;
    int s2_x2 = s2.x + s2.width;
    int s2_y1 = s2.y;
    int s2_y2 = s2.y + s2.height;

    // Check if the bounding boxes overlap in both x- and y-coordinates
    if (s1_x1 < s2_x2 && s1_x2 > s2_x1 && s1_y1 < s2_y2 && s1_y2 > s2_y1)
    {
        return true; // Squares are colliding
    }
    return false; // Squares are not colliding
}

using namespace std;
int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    int N = 30;
    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    int windowWidth = 640;
    int windowHeight = 480;
    int frames = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("GAME COSINE", // creates a window
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight, 0);

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return 1;
    }

    // create an array of N  rectangles
    Square rect[N];

    for (int i = 0; i < N; i++)
    {
        Square square;
        square = generate_random_square(window);

        int numberOfTries = 0;
        int j = 0;

        while (j < N && numberOfTries < triesNum)
        {
            Square previous_square = rect[j];

            if (check_collision(square, previous_square))
            {
                square = generate_random_square(window);
                numberOfTries++;
                j = -1;
                // printf("trueeeeee %d, %d, actual %d,%d,%d,%d prev %d,%d,%d,%d \n", j, i, square.color.r, square.color.g, square.color.b, square.color.a,  previous_square.color.r, previous_square.color.g, previous_square.color.b, previous_square.color.a);
                // printf("prev (%d, %d, %d) actual (%d, %d, %d)\n", previous_square.x, previous_square.y, previous_square.width, square.x, square.y, square.width);
            }
            j++;
        }

        // If there was not a match then remove the square
        if (numberOfTries >= triesNum)
        {
            // printf("position %d\n", numberOfTries);
            square.x = -1;
            square.y = -1;
            square.width = 0;
            square.height = 0;
        }

        // Add square
        rect[i] = square;
    }

    // Game loop
    Uint32 prevTicks = SDL_GetTicks();
    Uint32 start = SDL_GetTicks();
    while (true)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
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
        // --------------------

        for (int i = 0; i < N; i++)
        {
            //check for collision with the walls
            if (rect[i].x < 0 || rect[i].x > windowWidth - rect[i].width)
            {
                rect[i].angularSpeed *= -1;
            }
            if (rect[i].y < 0 || rect[i].y > windowHeight - rect[i].height)
            {
                rect[i].angularSpeed *= -1;
            }
            for (int j = 0; j < N; j++)
            {
                // Check for collision with other balls and swap velocities if collision occurs
                if (i != j)
                {
                    SDL_Rect rectangle1 = {rect[i].x, rect[i].y, rect[i].width, rect[i].height};
                    SDL_Rect rectangle2 = {rect[j].x, rect[j].y, rect[j].width, rect[j].height};
                    SDL_bool inter = SDL_HasIntersection(&rectangle1, &rectangle2);
                    if (inter)
                    {
                        rect[i].angularSpeed *= -1;
                        rect[j].angularSpeed *= -1;
                    }
                }
            }
            // Update angle
            //cout << "speed " << rect[i].angularSpeed << " angle " << rect[i].angle << " deltaTime " << deltaTime << "calc"<<rect[i].angularSpeed * deltaTime<<endl;
            rect[i].angle = rect[i].angle + (rect[i].angularSpeed * deltaTime);
            //cout<<"ANGLE"<<rect[i].angle <<endl;

            rect[i].x = rect[i].initialX + rect[i].radius * cos(rect[i].angle);
            rect[i].y = rect[i].initialY + rect[i].radius * sin(rect[i].angle);

            // Draw ball
            SDL_SetRenderDrawColor(renderer, rect[i].color.r, rect[i].color.g, rect[i].color.b, rect[i].color.a);
            SDL_Rect rectangle = {rect[i].x, rect[i].y, rect[i].width, rect[i].height};
            SDL_RenderFillRect(renderer, &rectangle);
        }

        // Swap buffers
        SDL_RenderPresent(renderer);

        frames++;
        Uint32 end = SDL_GetTicks();
        Uint32 elapsed = end - start;

        if (elapsed >= 1000)
        {
            double fps = frames / (elapsed / 1000.0);
            // printf("FPS: %f\n", fps);
            frames = 0;
            start = end;
        }
    }
    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
