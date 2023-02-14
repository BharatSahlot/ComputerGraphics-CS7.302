#include "Game.hpp"
#include "Engine/Camera.hpp"
#include "Engine/UI/Bar.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Objects/LoadingBar.hpp"
#include "Game/Worlds/LoadingWorld.hpp"
#include "Game/Worlds/StartMenuWorld.hpp"
#include "Game/Worlds/GameWorld.hpp"

Game::Game(std::shared_ptr<Window> window) : state(GameState::None)
{
    loadingWorld = new LoadingWorld(window, this);
    startMenuWorld = new StartMenuWorld(window, this);
    gameWorld = new GameWorld(window, this);

    SetGameState(GameState::Loading);
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
            startMenuWorld->GetResourceManager().StartLoading();
            SetActiveWorld(loadingWorld);
            return;
        case InMenu:
            gameWorld->GetResourceManager().StartLoading();
            SetActiveWorld(startMenuWorld);
            return;
        case InGame:
        case Paused:
        case Ended:
            SetActiveWorld(gameWorld);
            break;
        case None:
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
