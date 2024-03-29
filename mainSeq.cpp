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
#include <iostream>
#include <omp.h>
using namespace std;

#define numberOfTriesAllow 1000
#define maxRadiusMCU 20

/**
 * @brief Struct to represent a square
 *
 */
struct Square
{
    int x;              // x-coordinate of top left corner
    int y;              // y-coordinate of top left corner
    int initialX;       // Initial pos X
    int initialY;       // Initial pos Y
    int width;          // width of square
    int height;         // height of square
    float angle;        // Angle
    int radius;         // Radius
    float angularSpeed; // Angular speed
    SDL_Color color;    // Color
};

// Generate a random square within the bounds of the window
/**
 * @brief Generate a random square within the bounds of the window
 *
 * @param window window that stores the sizes of the simulation type SDL_Window
 * @return Square new square to be added to the simulation type Square
 */
Square generateRandomSquare(SDL_Window *window)
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
    square.radius = rand() % maxRadiusMCU + 5;
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
 * @param square1 Square 1 type Square
 * @param square2  Square 2 type Square
 * @return true if colliding
 * @return false if not colliding
 */
bool checkCollision(Square square1, Square square2)
{
    // Calculate the x- and y-coordinates of the bounding boxes
    int square1X1 = square1.x;
    int square1X2 = square1.x + square1.width;
    int square1Y1 = square1.y;
    int square1Y2 = square1.y + square1.height;
    int square2X1 = square2.x;
    int square2X2 = square2.x + square2.width;
    int square2Y1 = square2.y;
    int square2Y2 = square2.y + square2.height;

    // Check if the bounding boxes overlap in both x- and y-coordinates
    if (square1X1 < square2X2 && square1X2 > square2X1 && square1Y1 < square2Y2 && square1Y2 > square2Y1)
    {
        return true; // Squares are colliding
    }
    return false; // Squares are not colliding
}

int main(int argc, char *argv[])
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    int numberSquares = 30;
    // Get the number of squares from the command line
    if (argc > 1)
    {
        // validate if the argument is a number
        if (atoi(argv[1]) == 0)
        {
            printf("The argument must be a number\n");
            return 1;
        }
        numberSquares = atoi(argv[1]);
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
    Square squaresArray[numberSquares];
    // time to calculate speedup
    double timeInit = omp_get_wtime();

    // Generate N random squares
    for (int i = 0; i < numberSquares; i++)
    {
        Square square;
        square = generateRandomSquare(window);

        int numberOfTries = 0;
        int j = 0;
        // Check if the square collides with any of the previous squares
        while (j < numberSquares && numberOfTries < numberOfTriesAllow && j < i)
        {
            Square previousSquare = squaresArray[j];

            if (checkCollision(square, previousSquare))
            {
                // If the square collides with a previous square then generate a new square
                square = generateRandomSquare(window);
                numberOfTries++;
                j = -1;
            }
            j++;
        }

        // If there was not a match then remove the square (set x and y to -1 and size to 0)
        if (numberOfTries >= numberOfTriesAllow)
        {
            square.x = -1;
            square.y = -1;
            square.width = 0;
            square.height = 0;
        }

        // Add square
        squaresArray[i] = square;
    }

    // Game loop
    Uint32 prevTicks = SDL_GetTicks();
    Uint32 start = SDL_GetTicks();
    bool firstTime = true;
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
        for (int i = 0; i < numberSquares; i++)
        {
            // check for collision with the walls
            if (squaresArray[i].x < 0 || squaresArray[i].x > windowWidth - squaresArray[i].width)
            {
                squaresArray[i].angularSpeed *= -1;
            }
            if (squaresArray[i].y < 0 || squaresArray[i].y > windowHeight - squaresArray[i].height)
            {
                squaresArray[i].angularSpeed *= -1;
            }
            for (int j = 0; j < numberSquares; j++)
            {
                // Check for collision with other balls and swap velocities if collision occurs
                if (i != j)
                {
                    // next pos i
                    // Update angle
                    float angleI = squaresArray[i].angle + (squaresArray[i].angularSpeed * deltaTime);

                    // Update position
                    int posIx = squaresArray[i].initialX + squaresArray[i].radius * cos(angleI);
                    int posIy = squaresArray[i].initialY + squaresArray[i].radius * sin(angleI);

                    // next pos j
                    // Update angle
                    float angleJ = squaresArray[j].angle + (squaresArray[j].angularSpeed * deltaTime);

                    // Update position
                    int posJx = squaresArray[j].initialX + squaresArray[j].radius * cos(angleJ);
                    int posJy = squaresArray[j].initialY + squaresArray[j].radius * sin(angleJ);

                    SDL_Rect rectangleI = {posIx, posIy, squaresArray[i].width, squaresArray[i].height};
                    SDL_Rect rectangleJ = {posJx, posJy, squaresArray[j].width, squaresArray[j].height};
                    SDL_bool HasIntersectionNext = SDL_HasIntersection(&rectangleI, &rectangleJ);

                    // other pos i
                    // Update angle
                    float otherAngleI = squaresArray[i].angle + (squaresArray[i].angularSpeed * deltaTime * (-1));

                    // Update position
                    int otherPosIx = squaresArray[i].initialX + squaresArray[i].radius * cos(otherAngleI);
                    int otherPosIy = squaresArray[i].initialY + squaresArray[i].radius * sin(otherAngleI);

                    // other pos j
                    // Update angle
                    float otherAngleJ = squaresArray[j].angle + (squaresArray[j].angularSpeed * deltaTime * (-1));

                    // Update position
                    int otherPosJx = squaresArray[j].initialX + squaresArray[j].radius * cos(otherAngleJ);
                    int otherPosJy = squaresArray[j].initialY + squaresArray[j].radius * sin(otherAngleJ);

                    SDL_Rect otherRectangleI = {otherPosIx, otherPosIy, squaresArray[i].width, squaresArray[i].height};
                    SDL_Rect otherRectangleJ = {otherPosJx, otherPosJy, squaresArray[j].width, squaresArray[j].height};
                    SDL_bool otherHasIntersectionNext = SDL_HasIntersection(&otherRectangleI, &otherRectangleJ);

                    if (HasIntersectionNext && otherHasIntersectionNext)
                    {
                        squaresArray[i].width = squaresArray[i].width / 2;
                        squaresArray[i].height = squaresArray[i].height / 2;
                    }
                    else if (HasIntersectionNext)
                    {
                        squaresArray[i].angularSpeed *= -1;
                    }
                }
            }
        }

        // update the values
        for (int i = 0; i < numberSquares; i++)
        {
            // Update angle
            squaresArray[i].angle = squaresArray[i].angle + (squaresArray[i].angularSpeed * deltaTime);

            // Update position
            squaresArray[i].x = squaresArray[i].initialX + squaresArray[i].radius * cos(squaresArray[i].angle);
            squaresArray[i].y = squaresArray[i].initialY + squaresArray[i].radius * sin(squaresArray[i].angle);

            // Draw ball
            SDL_SetRenderDrawColor(renderer, squaresArray[i].color.r, squaresArray[i].color.g, squaresArray[i].color.b, squaresArray[i].color.a);
            SDL_Rect rectangle = {squaresArray[i].x, squaresArray[i].y, squaresArray[i].width, squaresArray[i].height};
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
        if (firstTime)
        {
            double timeFin = omp_get_wtime();
            double delta = timeFin - timeInit;
            cout << "Sequential program took " << delta << " seconds" << endl;
        }
        firstTime = false;
    }
    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
