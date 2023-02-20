#include "Model.hpp"
#include "Engine/Window/Window.hpp"
#include "assimp/material.h"

#include <iostream>

#include "Engine/World.hpp"
#include "assimp/vector3.h"
#include "glm/ext/matrix_transform.hpp"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <queue>

Model::Model(World* world) : hasModelBounds(false)
{
    this->world = world;
}

void Model::Setup()
{
    for(auto& mesh: meshes) mesh->Setup();
    for(auto model: children) model->Setup();
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Material> material;

    auto bmn = mesh->mAABB.mMin;
    auto bmx = mesh->mAABB.mMax;
    Bounds b = Bounds::FromMinMax(glm::vec3(bmn.x, bmn.y, bmn.z), glm::vec3(bmx.x, bmx.y, bmx.z));

    for(size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else
        {
            vertices.push_back(0);
            vertices.push_back(0);
        }
    }

    for(size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        aiString matName;
        mat->Get(AI_MATKEY_NAME, matName);

        std::string shaderPath = "Shaders/";
        shaderPath += matName.C_Str();
        shaderPath += ".fs";

        // std::cerr << "Adding shader to queue " << shaderPath << std::endl;
        material = world->GetResourceManager().AddInResourceQueue<Material>(shaderPath, ResourceLoadData<Material> {
            "Shaders/base.vs", shaderPath
        });

        // only one texture
        if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString file;
            mat->GetTexture(aiTextureType_DIFFUSE, 0, &file);

            std::string path = directory;
            path += '/';
            path += file.C_Str();
            auto tex = world->GetResourceManager().AddInResourceQueue<Texture>(path, ResourceLoadData<Texture> {
                path, GL_NEAREST
            });
            texture = tex;
        }
    }
    return new Mesh(vertices, indices, texture, material, b);
}

Bounds Model::GetModelBounds(Model& model)
{
    if(model.hasModelBounds) return model.modelBounds;

    Bounds mb = model.meshBounds;
    for(auto x: model.children)
    {
        Bounds b = GetModelBounds(*x);
        glm::vec3 p1 = x->transform.GetLocalModelMatrix() * glm::vec4(b.min, 1);
        glm::vec3 p2 = x->transform.GetLocalModelMatrix() * glm::vec4(b.max, 1);

        glm::vec3 mn(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
        glm::vec3 mx(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
        mb.Join(Bounds::FromMinMax(mn, mx));
    }
    model.modelBounds = mb;
    model.hasModelBounds = true;
    return mb;
}

int Model::Load(const std::string &path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate
            | aiProcess_GenNormals
            | aiProcess_GenBoundingBoxes
        );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp unable to import: " << importer.GetErrorString() << std::endl;
        return -1;
    }

    directory = path.substr(0, path.find_last_of('/'));

    bool first = true;
    std::queue<std::pair<aiNode*, Model*>> q({ { scene->mRootNode, nullptr } });
    while(!q.empty())
    {
        auto [node, par] = q.front();
        q.pop();

        auto md = this;
        if(!first) 
        {
            md = new Model(world);
            par->children.push_back(std::shared_ptr<Model>(md));
        }
        first = false;

        md->name = node->mName.C_Str();
        md->meshBounds = Bounds::FromExtents(glm::vec3(0), glm::vec3(0));

        aiVector3t<float> pos, rot, scale;
        node->mTransformation.Decompose(scale, rot, pos);

        md->transform.SetLocalScale(glm::vec3(scale.x, scale.y, scale.z));
        md->transform.SetLocalRotation(glm::vec3(rot.x, rot.y, rot.z));
        md->transform.SetLocalPosition(glm::vec3(pos.x, pos.y, pos.z));

        if(par) md->transform.SetParent(&par->transform);

        // std::cout << md->name << std::endl;
        // std::cout << pos.x << ' ' << pos.y << ' ' << pos.z << std::endl;
        // std::cout << rot.x << ' ' << rot.y << ' ' << rot.z << std::endl;
        // std::cout << scale.x << ' ' << scale.y << ' ' << scale.z << std::endl;
        // std::cout << std::endl;

        for(size_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            auto mesh1 = std::shared_ptr<Mesh>(ProcessMesh(mesh, scene));
            mesh1->name = md->name + '.' + std::to_string(i);
            md->meshes.push_back(mesh1);

            if(i == 0) md->meshBounds = mesh1->GetBounds();
            else md->meshBounds.Join(mesh1->GetBounds());

            if(mesh->mNumVertices == 4)
            {
            }
        }

        for(size_t i = 0; i < node->mNumChildren; i++)
        {
            q.push({node->mChildren[i], md});
        }
    }
    transform.SetLocalScale(glm::vec3(0.1, 0.1, 0.1));
    return 0;
}

void Model::Render(const glm::mat4& proj, const glm::mat4& view, const glm::mat4& model)
{
    for(const auto& mesh: meshes)
    {
        mesh->Render(model, view, proj);
    }

    for(auto model: children) model->Render(view, proj);
}
