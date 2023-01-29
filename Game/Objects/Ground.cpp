#include "Ground.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Globals.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <iostream>
#include <cstdlib>

Ground::Ground(float tileSize, int yCount, int xCount, float xSpeed) : Object("ground", Plane, nullptr)
{
    sheet = Texture::MakeTexture("Game/Assets/Ground_Tileset.png", GL_NEAREST);
    if(sheet == nullptr)
    {
        std::cerr << "Unable to load ground tilesheet image file" << std::endl;
    }

    Shader* vs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* fs = Shader::MakeShader("Shaders/ground.fs", GL_FRAGMENT_SHADER);
    material = std::shared_ptr<Material>(Material::MakeMaterial(vs, fs));

    floorTiles = {6, 7};
    soilTiles = {22};
    this->tileSize = tileSize;
    this->xCount = xCount;
    this->yCount = yCount;
    this->xSpeed = xSpeed;

    texXCount = 21;
    texYCount = 15;

    for(int i = 0; i < xCount; i++)
    {
        std::vector<int> vec(yCount, 0);
        for(int j = 0; j < yCount; j++)
        {
            vec[j] = soilTiles[0];
            if(j == yCount - 1)
            {
                vec[j] = floorTiles[rand() % floorTiles.size()];
            }
        }
        state.push_back(vec);
    }

    transform->SetWorldPosition((float)tileSize / 2.f, (float)tileSize / 2.f, 0.1f);
}

void Ground::Start() {}

void Ground::Tick(const Window &window, float deltaTime)
{
    glm::vec3 offset(xSpeed * deltaTime, 0, 0);
    glm::vec3 pos(transform->GetWorldPosition());

    if(pos.x < -tileSize)
    {
        pos.x += tileSize;
        state.pop_front();

        std::vector<int> vec(yCount, 0);
        for(int j = 0; j < yCount; j++)
        {
            vec[j] = soilTiles[0];
            if(j == yCount - 1)
            {
                vec[j] = floorTiles[rand() % floorTiles.size()];
            }
        }
        state.push_back(vec);
    }

    transform->SetWorldPosition(pos + offset);
}

void Ground::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    sheet->Use(0);

    material->Use();
    material->SetInt("texture1", 0);

    glm::vec2 texDims = sheet->GetDims();
    material->SetVec2("texDims", texDims);
    material->SetVec2("spriteDims", glm::vec2(texDims.x / texXCount, texDims.y / texYCount));

    material->SetInt("texXCount", texXCount);
    material->SetInt("texYCount", texYCount);

    glm::mat4 model = transform->GetModelMatrix();
    for(int i = 0; i < xCount; i++)
    {
        for(int j = 0; j < yCount; j++)
        {
            material->SetInt("index", state[i][j]);
            material->SetUniformMat4("view", viewMat);
            material->SetUniformMat4("proj", projMat);

            glm::mat4 m = glm::translate(model, glm::vec3(i * tileSize, j * tileSize, 0.f));
            m = glm::scale(m, glm::vec3(tileSize, tileSize, 1));
            material->SetUniformMat4("model", m);

            mesh->Render();
        }
    }
}
