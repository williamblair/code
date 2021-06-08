// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
{
    mPaddleDir[0] = mPaddleDir[1] = 0;    
}

bool Game::Initialize()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    // Create an SDL Window
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100,    // Top left x-coordinate of window
        100,    // Top left y-coordinate of window
        1024,    // Width of window
        768,    // Height of window
        0        // Flags (0 for no flags set)
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
    //// Create SDL renderer
    mRenderer = SDL_CreateRenderer(
        mWindow, // Window to create renderer for
        -1,         // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    //
    mPaddlePos[0].x = 10.0f;
    mPaddlePos[0].y = 768.0f / 2.0f;
    mPaddlePos[1].x = 1024.0f - 10.0f - thickness;
    mPaddlePos[1].y = 768.0f / 2.0f;
    // place a couple of balls
    mBalls.push_back({
        {1024.0f / 2.0f, // ball x
         768.0f / 2.0f}, // ball y
        {-200.0f,        // velocity x
          235.0f}        // velocity y
    });
    mBalls.push_back({
        {1024.0f / 2.0f + 100.0f, // ball x
         768.0f / 2.0f - 50.0f},  // ball y
        {200.0f,                  // velocity x
         235.0f}                  // velocity y
    });
    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // If we get an SDL_QUIT event, end loop
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    
    // Get state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    // Update paddle direction based on W/S keys
    mPaddleDir[0] = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir[0] -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir[0] += 1;
    }

    // Update paddle2 direction based on UP/DOWN keys
    mPaddleDir[1] = 0;
    if (state[SDL_SCANCODE_UP])
    {
        mPaddleDir[1] -= 1;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        mPaddleDir[1] += 1;
    }
}

void Game::UpdateGame()
{
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();
    
    // Update paddle position based on direction
    for (int i = 0; i < 2; ++i)
    {
        if (mPaddleDir[i] != 0)
        {
            mPaddlePos[i].y += mPaddleDir[i] * 300.0f * deltaTime;
            // Make sure paddle doesn't move off screen!
            if (mPaddlePos[i].y < (paddleH/2.0f + thickness))
            {
                mPaddlePos[i].y = paddleH/2.0f + thickness;
            }
            else if (mPaddlePos[i].y > (768.0f - paddleH/2.0f - thickness))
            {
                mPaddlePos[i].y = 768.0f - paddleH/2.0f - thickness;
            }
        }
    }

    // Update each ball instance
    for (Ball& ball : mBalls)
    {
        Vector2& ballPos = ball.pos;
        Vector2& ballVel = ball.vel;

        // Update ball position based on ball velocity
        ballPos.x += ballVel.x * deltaTime;
        ballPos.y += ballVel.y * deltaTime;
        
        // Bounce if needed
        // Did we intersect with the paddle?
        float diff = mPaddlePos[0].y - ballPos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            ballPos.x <= 25.0f && ballPos.x >= 20.0f &&
            // The ball is moving to the left
            ballVel.x < 0.0f)
        {
            ballVel.x *= -1.0f;
        }
        // Did we intersect with the second paddle?
        diff = mPaddlePos[1].y - ballPos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            // thickness*2 because we account for both the paddle and ball thickness
            ballPos.x >= 1024.0f - 10.0f - (thickness*2) && 
            ballPos.x <= 1024.0f - thickness &&
            // The ball is moving to the right
            ballVel.x > 0.0f)
        {
            ballVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (ballPos.x <= 0.0f)
        {
            mIsRunning = false;
            break; // no need to keep checking other balls
        }
        else if (ballPos.x >= 1024.0f)
        {
            mIsRunning = false;
            break; // no need to keep checking other balls
        }
        
        // Did the ball collide with the top wall?
        if (ballPos.y <= thickness && ballVel.y < 0.0f)
        {
            ballVel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (ballPos.y >= (768 - thickness) &&
            ballVel.y > 0.0f)
        {
            ballVel.y *= -1;
        }
    } // end for (Ball& ball : balls)
}

void Game::GenerateOutput()
{
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,          // R
        0,          // G 
        255,        // B
        255         // A
    );
    
    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draw walls
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    // Draw top wall
    SDL_Rect wall{
        0,          // Top left x
        0,          // Top left y
        1024,       // Width
        thickness   // Height
    };
    SDL_RenderFillRect(mRenderer, &wall);
    
    // Draw bottom wall
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);
    
    // Draw paddles
    for (int i = 0; i < 2; ++i)
    {
        SDL_Rect paddle{
            static_cast<int>(mPaddlePos[i].x),
            static_cast<int>(mPaddlePos[i].y - paddleH/2),
            thickness,
            static_cast<int>(paddleH)
        };
        SDL_RenderFillRect(mRenderer, &paddle);
    }
    
    // Draw balls
    for (Ball& ball : mBalls)
    {
        SDL_Rect ballRect{    
            static_cast<int>(ball.pos.x - thickness/2),
            static_cast<int>(ball.pos.y - thickness/2),
            thickness,
            thickness
        };
        SDL_RenderFillRect(mRenderer, &ballRect);
    }
    
    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
