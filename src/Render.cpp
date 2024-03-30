#include "App.h"

#include "Game.h"
#include "Delay.h"

#include <string>

bool paused = true;

std::shared_ptr<Snake> snake;
std::shared_ptr<Food> food;

Delay delay(0.075f);

void resetGame()
{
    snake->body = {Vec2i{6, 9}, Vec2i{5, 9}, Vec2i{4, 9}};
    snake->direction = {1, 0};
    snake->grow = false;

    food->position = food->GenerateRandomPosition(snake->body);

    delay.counter = 0.f;
}

void App::Setup()
{
    snake = std::make_shared<Snake>(Snake(20, 30));
    food = std::make_shared<Food>(Food(20, 30));
    delay.start = true;
}

void App::Update(SDL_Event &event, bool &quit, float deltaTime)
{
    while (SDL_PollEvent(&event))
    {
        // ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quit = 1;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                quit = 1;

            else if (event.key.keysym.sym == SDLK_k)
                SDL_Log("fps = %f", 1 / deltaTime);

            snakeGameKeyEvents(event, paused);
            break;
        }
    }

    if (paused)
        return;

    if (delay.Update(deltaTime))
    {
        snake->Update();
        delay.start = true;
    }

    if (snake->body[0] == food->position)
    {
        food->position = food->GenerateRandomPosition(snake->body);
        snake->grow = true;
        SDL_SetWindowTitle(window, std::to_string(snake->body.size() - 2).data());
    }

    if (snake->body[0].x == snake->cellCount)
        snake->body[0].x = 0;
    if (snake->body[0].x == -1)
        snake->body[0].x = snake->cellCount - 1;

    if (snake->body[0].y == snake->cellCount)
        snake->body[0].y = 0;
    if (snake->body[0].y == -1)
        snake->body[0].y = snake->cellCount - 1;


    std::deque<Vec2i> headlessBody = snake->body;
    headlessBody.pop_front();
    for (size_t i = 0; i < headlessBody.size(); i++)
    {
        if (snake->body[0] == headlessBody[i])
        {
            resetGame();
            paused = true;
        }
    }
}

void App::Draw()
{
    // ImGuiIO &io = this->ImguiNewFrame();
    this->SetRenderDrawColor({22, 22, 22});
    SDL_RenderClear(renderer);

    snake->Draw(renderer);
    food->Draw(renderer);

    // this->ImguiRender();
    SDL_RenderPresent(renderer);
}