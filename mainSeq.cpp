/**
 * @file main.cpp
 * @author Raul Jimenez
 * @author Bryann Alfaro
 * @author Donaldo Garcia
 * @brief Main file of the project that has the screen saver simulation secuential.
 * @version 0.1
 * @date 2023-03-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include<iostream>

#define triesNum 10
#define maxRadius 20

/**
 * @brief Struct to represent a square
 * 
 */
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
/**
 * @brief Generate a random square within the bounds of the window
 * 
 * @param window window that stores the sizes of the simulation type SDL_Window
 * @return Square new square to be added to the simulation type Square
 */
Square generate_random_square(SDL_Window *window)
{
    Square square;
    int size = (rand() % 45) + 5;
    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);
    square.x = (rand() % (winWidth - size * 2)) + size;  // module of window minues size times 2 plus the size to ensure the square fits within the window 
    square.y = (rand() % (winHeight - size * 2)) + size; // module of window minues size times 2 plus the size to ensure the square fits within the window 
    square.initialX = square.x;
    square.initialY = square.y;
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
    square.width = size;
    square.height = square.width;
    square.color = {static_cast<unsigned char>(rand() % 256),
                    static_cast<unsigned char>(rand() % 256),
                    static_cast<unsigned char>(rand() % 256),
                    255}; // Generate a random color with full alpha
    return square;
}

/**
 * @brief Check if two squares are colliding
 * 
 * @param s1 Square 1 type Square
 * @param s2  Square 2 type Square
 * @return true if colliding
 * @return false if not colliding
 */
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
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    int N = 30;
    // Get the number of squares from the command line
    if (argc > 1)
    {
        // validate if the argument is a number
        if (atoi(argv[1]) == 0)
        {
            printf("The argument must be a number\n");
            return 1;
        }
        N = atoi(argv[1]);
    }
    // Window dimensions
    int windowWidth = 640;
    int windowHeight = 480;
    // frames for fps
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

    // create an array of N squares
    Square rect[N];

    // Generate N random squares
    for (int i = 0; i < N; i++)
    {
        Square square;
        square = generate_random_square(window);

        int numberOfTries = 0;
        int j = 0;
        // Check if the square collides with any of the previous squares
        while (j < N && numberOfTries < triesNum)
        {
            Square previous_square = rect[j];

            if (check_collision(square, previous_square))
            {
                // If the square collides with a previous square then generate a new square
                square = generate_random_square(window);
                numberOfTries++;
                j = -1;
            }
            j++;
        }

        // If there was not a match then remove the square (set x and y to -1 and size to 0)
        if (numberOfTries >= triesNum)
        {
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
            rect[i].angle = rect[i].angle + (rect[i].angularSpeed * deltaTime);

            // Update position
            rect[i].x = rect[i].initialX + rect[i].radius * cos(rect[i].angle);
            rect[i].y = rect[i].initialY + rect[i].radius * sin(rect[i].angle);

            // Draw ball
            SDL_SetRenderDrawColor(renderer, rect[i].color.r, rect[i].color.g, rect[i].color.b, rect[i].color.a);
            SDL_Rect rectangle = {rect[i].x, rect[i].y, rect[i].width, rect[i].height};
            SDL_RenderFillRect(renderer, &rectangle);
        }

        // Swap buffers
        SDL_RenderPresent(renderer);

        // FPS
        frames++;
        Uint32 end = SDL_GetTicks();
        Uint32 elapsed = end - start;

        if (elapsed >= 1000)
        {
            double fps = frames / (elapsed / 1000.0);
            printf("FPS: %f\n", fps);
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
