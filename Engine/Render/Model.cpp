#include "Model.hpp"
#include "Engine/Window/Window.hpp"
#include "assimp/material.h"

#include <iostream>

#include "Engine/World.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <queue>

Model::Model(World* world)
{
    this->world = world;
}

void Model::Setup()
{
    for(auto& mesh: meshes) mesh.Setup();
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Material> material;

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
                path, GL_LINEAR
            });
            texture = tex;
        }
    }
    return Mesh(vertices, indices, texture, material);
}

int Model::Load(const std::string &path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate
            // | aiProcess_FlipUVs
            | aiProcess_GenNormals
            | aiProcess_PreTransformVertices
            | aiProcess_FixInfacingNormals
            | aiProcess_FlipWindingOrder
        );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp unable to import: " << importer.GetErrorString() << std::endl;
        return -1;
    }

    directory = path.substr(0, path.find_last_of('/'));

    std::queue<aiNode*> q({ scene->mRootNode });

    while(!q.empty())
    {
        aiNode* node = q.front();
        q.pop();

        for(size_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for(size_t i = 0; i < node->mNumChildren; i++)
        {
            q.push(node->mChildren[i]);
        }
    }
    return 0;
}

void Model::Render(const glm::mat4& view, const glm::mat4& proj)
{
    for(const Mesh& mesh: meshes)
    {
        mesh.Render(glm::mat4(1), view, proj);
    }
}
