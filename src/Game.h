#pragma once

#include "util.h"
#include "SDL3/SDL_render.h"

#include <deque>
#include <memory>
#include <random>

class Food
{
private:
    int cellSize, cellCount;
public:
    Vec2i position;
    Food(int cellSize, int cellCount)
        : cellSize(cellSize), cellCount(cellCount)
    {
        position = {5, 6};
    }

    void Draw(SDL_Renderer *&renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_FRect rect = {position.x * (float)cellSize, position.y * (float)cellSize, (float)cellSize, (float)cellSize};
        SDL_RenderRect(renderer, &rect);
    }

    Vec2i GenerateRandomPosition(std::deque<Vec2i> &snakeBody)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, int(cellCount - 1));

        int x = dist(rng);
        int y = dist(rng);
        Vec2i position = Vec2i{x, y};

        while (true)
        {
            bool successful = true;
            for (size_t i = 0; i < snakeBody.size(); i++)
            {
                if (snakeBody[i] == position)
                {
                    x = dist(rng);
                    y = dist(rng);
                    position = Vec2i{x, y};
                    successful = false;
                }
            }

            if (successful)
                break;
        }

        return position;
    }
};

class Snake
{
public:
    int cellSize, cellCount;
    std::deque<Vec2i> body;
    Vec2i direction;
    bool grow;

    Snake(int cellSize, int cellCount)
        : cellSize(cellSize), cellCount(cellCount)
    {
        body = {Vec2i{6, 9}, Vec2i{5, 9}, Vec2i{4, 9}};
        direction = {1, 0};
        grow = false;
    }

    void Update()
    {
        if (grow)
        {
            body.push_front(body[0] + direction);
            grow = false;
            return;
        }

        body.pop_back();
        body.push_front(body[0] + direction);
    }

    void Draw(SDL_Renderer *&renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t i = 0; i < body.size(); i++)
        {
            SDL_FRect rect = {body[i].x * (float)cellSize, body[i].y * (float)cellSize, (float)cellSize, (float)cellSize};
            SDL_RenderRect(renderer, &rect);
        }
    }
};

extern std::shared_ptr<Snake> snake;
void snakeGameKeyEvents(SDL_Event &event, bool& paused)
{
    if (event.key.keysym.sym == SDLK_UP && snake->direction.y != 1)
    {
        snake->direction = {0, -1};
        paused = false;
    }

    if (event.key.keysym.sym == SDLK_DOWN && snake->direction.y != -1)
    {
        snake->direction = {0, 1};
        paused = false;
    }

    if (event.key.keysym.sym == SDLK_LEFT && snake->direction.x != 1)
    {
        snake->direction = {-1, 0};
        paused = false;
    }

    if (event.key.keysym.sym == SDLK_RIGHT && snake->direction.x != -1)
    {
        snake->direction = {1, 0};
        paused = false;
    }
}