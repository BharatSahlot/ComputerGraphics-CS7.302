#ifndef MODEL_H
#define MODEL_H

#include "Engine/Components/Transform.hpp"
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
        void Render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model = glm::mat4(1));

        std::string GetName() const { return name; }
        Transform GetTransform() const { return transform; }
        const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return meshes; }
        const std::vector<std::shared_ptr<Model>>& GetChildren() const { return children; }

        Bounds GetMeshBounds() const { return meshBounds; }
        Bounds GetModelBounds(Model& model);

    private:
        Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

        std::string name;
        Transform transform;
        World* world;
        std::string directory;

        bool hasModelBounds;
        Bounds modelBounds;
        Bounds meshBounds;

        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Model>> children;
};

#endif
