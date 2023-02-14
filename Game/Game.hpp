#ifndef GAME_H
#define GAME_H

#include "Engine/UI/Bar.hpp"
#include "Engine/World.hpp"

class LoadingWorld;

enum GameState
{
    Loading,
    InMenu,
    InGame,
    Paused,
    Ended
};

class Game
{
    public:
        Game(std::shared_ptr<Window> window);
        ~Game();

        void Tick(float deltaTime);
        void Render();

        void SetGameState(GameState state);

        World& GetStartMenuWorld() const { return *startMenuWorld; }

        GameState GetState() const { return state; }

    private:
        void SetActiveWorld(World* world);

        GameState state;

        World* activeWorld;
        LoadingWorld* loadingWorld;
        World* startMenuWorld;
        World* gameWorld;
};

#endif
