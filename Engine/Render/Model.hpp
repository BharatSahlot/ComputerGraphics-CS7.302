#ifndef MODEL_H
#define MODEL_H

#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Texture.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <vector>

class World;

class Model
{
    public:
        Model(World* world);
        int Load(const std::string& path);
        void Setup();
        void Render(const Material &material, const glm::mat4& view, const glm::mat4& proj);

    private:
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        World* world;
        std::string directory;
        std::vector<Mesh> meshes;
};

#endif
