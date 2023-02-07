#ifndef LEVELS_H
#define LEVELS_H

#include "Game/Levels/Level.hpp"

class Level1 : public Level
{
    using Level::Level;
    public:
        int Load() override;
};

class Level2 : public Level
{
    using Level::Level;
    public:
        int Load() override;
};

class Level3 : public Level
{
    using Level::Level;
    public:
        int Load() override;
};

#endif
