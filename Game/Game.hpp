#ifndef GAME_H
#define GAME_H

#include "Engine/UI/Bar.hpp"
#include "Engine/World.hpp"

class LoadingWorld;
class StartMenuWorld;

enum GameState
{
    None,
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

        StartMenuWorld& GetStartMenuWorld() const { return *startMenuWorld; }
        World& GetGameWorld() const { return *gameWorld; }

        GameState GetState() const { return state; }

    private:
        void SetActiveWorld(World* world);

        GameState state;

        World* activeWorld;
        LoadingWorld* loadingWorld;
        StartMenuWorld* startMenuWorld;
        World* gameWorld;
};

#endif
