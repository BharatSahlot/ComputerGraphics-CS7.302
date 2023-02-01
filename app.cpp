#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Font.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Utils/Random.hpp"
#include "GLFW/glfw3.h"
#include "Game/Levels/Level.hpp"
#include "Game/Levels/Levels.hpp"
#include "Game/Objects/Text.hpp"
#include "Game/Player/Player.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

#include "Globals.hpp"

std::shared_ptr<Mesh> Plane;
std::shared_ptr<Material> BasicMat;
std::shared_ptr<Material> BasicTexMat;
std::shared_ptr<Material> TextMat;
std::shared_ptr<Material> SpriteSheetMat;

Font* font;

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;
    srand(time(0));
    Random::Init();

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::mat4(1.0f);

    Window* window = Window::Create(800, 600, "pls");
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);
    Shader* texFrag = Shader::MakeShader("Shaders/tex.fs", GL_FRAGMENT_SHADER);

    Shader* textVert = Shader::MakeShader("Shaders/text.vs", GL_VERTEX_SHADER);
    Shader* textFrag = Shader::MakeShader("Shaders/text.fs", GL_FRAGMENT_SHADER);

    Shader* spriteSheetFrag = Shader::MakeShader("Shaders/spritesheet.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    BasicMat = std::shared_ptr<Material>(mat);

    BasicTexMat = std::shared_ptr<Material>(
            Material::MakeMaterial(baseVert, texFrag));

    TextMat = std::shared_ptr<Material>(
            Material::MakeMaterial(textVert, textFrag));

    SpriteSheetMat = std::shared_ptr<Material>(Material::MakeMaterial(baseVert, spriteSheetFrag));

    std::vector<float> vertices({
            0.5f,  0.5f, 0.0f, 1.f, 1.f,  // top right
            0.5f, -0.5f, 0.0f, 1.f, 0.f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.f, 0.f, // bottom left
            -0.5f,  0.5f, 0.0f, 0.f, 1.f
    });

    std::vector<int> indices({
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    });

    Mesh* mesh = new Mesh(vertices, indices, false);

    Plane = std::shared_ptr<Mesh>(mesh);

    window->Init();

    LevelSettings levelSettings1;
    levelSettings1.duration = 20.f;
    levelSettings1.speedModifier = 1.f;
    levelSettings1.zapperSpawnInterval = 7.f;
    levelSettings1.zapperSpawnIntervalVar = 2.f;
    levelSettings1.zapperHeight = 2.5f;
    levelSettings1.zapperHeightVar = 0.5f;
    levelSettings1.zapperRotSpeed = 30.f;
    levelSettings1.zapperRotSpeedVar = 10.f;
    levelSettings1.zapperYSpeed = 3.f;
    levelSettings1.zapperYSpeedVar = 2.f;
    levelSettings1.zapperCol = glm::vec3(0.7f, 0.7f, 1.f);
    levelSettings1.coinRadius = 0.5f;
    levelSettings1.coinSpawnInterval = 8.f;
    levelSettings1.coinSpawnIntervalVar = 2.f;
    levelSettings1.coinSpawnRadius = 1.f;
    levelSettings1.coinSpawnRadiusVar = 0.5f;


    LevelSettings levelSettings2;
    levelSettings2.duration = 25.f;
    levelSettings2.speedModifier = 1.f;
    levelSettings2.zapperSpawnInterval = 5.f;
    levelSettings2.zapperSpawnIntervalVar = 2.f;
    levelSettings2.zapperHeight = 3.5f;
    levelSettings2.zapperHeightVar = 0.5f;
    levelSettings2.zapperRotSpeed = 30.f;
    levelSettings2.zapperRotSpeedVar = 10.f;
    levelSettings2.zapperYSpeed = 3.f;
    levelSettings2.zapperYSpeedVar = 2.f;
    levelSettings2.zapperCol = glm::vec3(0.7f, 0.7f, 1.f);
    levelSettings2.coinRadius = 0.5f;
    levelSettings2.coinSpawnInterval = 8.f;
    levelSettings2.coinSpawnIntervalVar = 2.f;
    levelSettings2.coinSpawnRadius = 1.f;
    levelSettings2.coinSpawnRadiusVar = 0.5f;

    LevelSettings levelSettings3;
    levelSettings3.duration = 30.f;
    levelSettings3.speedModifier = 1.f;
    levelSettings3.zapperSpawnInterval = 4.f;
    levelSettings3.zapperSpawnIntervalVar = 1.f;
    levelSettings3.zapperHeight = 4.f;
    levelSettings3.zapperHeightVar = 0.5f;
    levelSettings3.zapperRotSpeed = 30.f;
    levelSettings3.zapperRotSpeedVar = 10.f;
    levelSettings3.zapperYSpeed = 3.f;
    levelSettings3.zapperYSpeedVar = 2.f;
    levelSettings3.zapperCol = glm::vec3(1.f, 0.0f, 0.2f);
    levelSettings3.coinRadius = 0.5f;
    levelSettings3.coinSpawnInterval = 8.f;
    levelSettings3.coinSpawnIntervalVar = 2.f;
    levelSettings3.coinSpawnRadius = 1.f;
    levelSettings3.coinSpawnRadiusVar = 0.5f;

    Level* levels[] = {
        new Level1(camera, levelSettings1),
        new Level2(camera, levelSettings2),
        new Level3(camera, levelSettings3),
    };

    Timer startUpTimer;
    startUpTimer.Start();
    levels[0]->Load();
    levels[1]->Load();
    levels[2]->Load();
    std::cerr << "Load time: " << startUpTimer.TimeSinceStart() << std::endl;

    int currentLevel = 0;

    font = Font::LoadFont("Game/Assets/font.ttf", 96);

    bool gameEnded = false;
    bool gameWon = false;

    Text gameEndText(-0.2f, 0.9f, 1.5f, glm::vec3(1, 1, 1));
    Text gameEndEnterText(-0.22f, 1.0f, 0.5f, glm::vec3(0.4, 0.4, 0.4));
    Text gameEndScoreText(-0.21f, 1.1f, 0.75f, glm::vec3(1, 1, 1));

    gameEndEnterText.SetText("Press enter to close the game.");

    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](const Window& window) -> bool {
        float delta = timer.Tick();

        if(gameEnded)
        {
            windowFade += delta;
            if(windowFade >= 1.f) windowFade = 1.f;

            gameEndText.Tick(window, delta);
            gameEndEnterText.Tick(window, delta);
            gameEndScoreText.Tick(window, delta);

            gameEndText.Render(camera->view, camera->Proj());
            gameEndEnterText.Render(camera->view, camera->Proj());
            gameEndScoreText.Render(camera->view, camera->Proj());

            if(window.GetKey(GLFW_KEY_ENTER)) return true;
            return false;
        }

        if(!levels[currentLevel]->started)
        {
            levels[currentLevel]->started = true;
            levels[currentLevel]->Start();
        }

        levels[currentLevel]->Tick(window, delta);
        levels[currentLevel]->Render(camera->view, camera->Proj());

        if(levels[currentLevel]->hasEnded)
        {
            bool playerDied = levels[currentLevel]->playerDied;
            levels[currentLevel]->Unload();
            if(playerDied)
            {
                gameEndText.Start();
                gameEndEnterText.Start();
                gameEndScoreText.Start();

                std::string txt = "Coins collected: ";
                txt += std::to_string(levels[currentLevel]->coinsCollected);
                gameEndScoreText.SetText(txt);

                gameEnded = true;
                gameWon = false;
                gameEndText.SetText("You lost!");
            }
            delete levels[currentLevel];
            currentLevel++;
        } else if(currentLevel >= 3)
        {
            gameEndText.Start();
            gameEndText.SetText("You won!");
            gameEndEnterText.Start();
            gameEndScoreText.Start();

            std::string txt = " Coins collected: ";
            txt += std::to_string(levels[currentLevel - 1]->coinsCollected);
            gameEndScoreText.SetText(txt);
            gameEnded = true;
            gameWon = true;
        }
        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
