#ifndef GAME_H
#define GAME_H

#include "Engine/UI/Bar.hpp"
#include "Engine/World.hpp"

class LoadingWorld;
class StartMenuWorld;
class GameWorld;
class GameOverWorld;

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

        std::string deathReason;

        void Tick(float deltaTime);
        void Render();

        void SetGameState(GameState state);
        void GameOver(std::string reason);

        StartMenuWorld& GetStartMenuWorld() const { return *startMenuWorld; }
        GameWorld& GetGameWorld() const { return *gameWorld; }

        GameState GetState() const { return state; }

    private:
        void SetActiveWorld(World* world);

        GameState state;

        World* activeWorld;
        LoadingWorld* loadingWorld;
        StartMenuWorld* startMenuWorld;
        GameWorld* gameWorld;
        GameOverWorld* gameOverWorld;
};

#endif
