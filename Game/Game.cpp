#include "Game.hpp"
#include "Engine/Camera.hpp"
#include "Engine/UI/Bar.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Objects/LoadingBar.hpp"
#include "Game/Worlds/LoadingWorld.hpp"

Game::Game(std::shared_ptr<Window> window) : state(GameState::Loading)
{
    loadingWorld = new LoadingWorld(window, this);
    startMenuWorld = new World(window);
    gameWorld = new World(window);

    activeWorld = loadingWorld;
    loadingWorld->Start();
}

void Game::Tick(float deltaTime)
{
    static float fillAmount = 0.f;

    fillAmount += deltaTime;

    activeWorld->Tick(deltaTime);
}

void Game::Render()
{
    activeWorld->Render();
}

void Game::SetGameState(GameState state)
{
    if(state == this->state) return;

    this->state = state;
    switch (state) {
    case Loading:
        SetActiveWorld(loadingWorld);
        return;
    case InMenu:
        SetActiveWorld(startMenuWorld);
        return;
    case InGame:
    case Paused:
    case Ended:
        SetActiveWorld(gameWorld);
      break;
    }
}

void Game::SetActiveWorld(World* world)
{
    if(world == activeWorld) return;

    activeWorld = world;
    world->Start();
}

Game::~Game()
{
    delete loadingWorld;
    delete startMenuWorld;
    delete gameWorld;
}
