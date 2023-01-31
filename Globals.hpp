#ifndef GLOBALS_H
#define GLOBALS_H

#include "Engine/Render/Font.hpp"
#include "Engine/Render/Mesh.hpp"
#include <memory>

const float GROUND = 0.0f;
const float GROUND_HEIGHT = 1.f;

const float CEILING = 9.f;
const float CEILING_HEIGH = 1.f;

const float GRAVITY = -10.f;
const float NITRO = 15.f - GRAVITY;

const float MAX_Y_VELOCITY = 7.5f;

const int SPRITE_ANIM_TIME_PER_FRAME = 50; // milliseconds

struct GameState
{
};

extern GameState State;
extern std::shared_ptr<Mesh> Plane;
extern std::shared_ptr<Material> BasicMat;
extern std::shared_ptr<Material> BasicTexMat;
extern std::shared_ptr<Material> TextMat;

extern Font* font;

#endif
