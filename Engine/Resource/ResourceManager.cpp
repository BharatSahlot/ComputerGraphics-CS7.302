#include "ResourceManager.hpp"
#include "Engine/Render/Material.hpp"
#include "GLFW/glfw3.h"
#include <iostream>
#include <sstream>

ResourceManager* ResourceManager::CreateResourceManager(GLFWwindow* window)
{
    ResourceManager* manager = new ResourceManager;

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    GLFWwindow* context = glfwCreateWindow(400, 400, "resource manager hidden window", NULL, window);

    if(context == NULL)
    {
        std::cerr << "Unable to create hidden context for resource manager" << std::endl;
        delete(manager);
        return nullptr;
    }

    manager->context = context;
    return manager;
}

ResourceManager::~ResourceManager()
{
    if(context) glfwDestroyWindow(context);
}

void ResourceManager::StartLoading()
{
    loaderThread = std::thread([&]() {
        Loader();
    });
}

std::string ResourceManager::GetLoadStatus() const
{
    std::stringstream ss;
    if(texturesLoaded < totalTextures)
    {
        ss << "Loading textures (" << texturesLoaded << "/" << totalTextures << ")";
        return ss.str();
    }

    if(materialsLoaded < totalMaterials)
    {
        ss << "Loading materials (" << materialsLoaded << "/" << totalMaterials << ")";
        return ss.str();
    }
    return "Loading complete";
}

bool ResourceManager::HasLoadingFinished() const { return texturesLoaded == totalTextures && materialsLoaded == totalMaterials; }

void ResourceManager::Load()
{
    loaderThread.join();
}

void ResourceManager::Loader()
{
    glfwMakeContextCurrent(context);

    totalTextures = textureQueue.size();
    totalMaterials = materialQueue.size();

    while(!textureQueue.empty())
    {
        auto [name, file] = textureQueue.front();
        textureQueue.pop();

        if(textureMap.count(name))
        {
            texturesLoaded++;
            continue;
        }

        std::shared_ptr<Texture> texture(Texture::MakeTexture(file));
        textureMap[name] = texture;
        texturesLoaded++;
    }

    while(!materialQueue.empty())
    {
        auto [name, data] = materialQueue.front();
        materialQueue.pop();

        if(materialMap.count(name))
        {
            materialsLoaded++;
            continue;
        }

        if(!shaderMap.count(data.vertexShaderFile))
        {
            shaderMap[data.vertexShaderFile] = Shader::MakeShader(data.vertexShaderFile.c_str(), GL_VERTEX_SHADER);
        }

        if(!shaderMap.count(data.fragmentShaderFile))
        {
            shaderMap[data.fragmentShaderFile] = Shader::MakeShader(data.fragmentShaderFile.c_str(), GL_FRAGMENT_SHADER);
        }

        std::shared_ptr<Material> material(Material::MakeMaterial(shaderMap.at(data.vertexShaderFile), shaderMap.at(data.fragmentShaderFile)));
        materialMap[name] = material;
        materialsLoaded++;
    }
}

template<>
void ResourceManager::AddInResourceQueue<Texture>(const std::string& name, const std::string &file)
{
    textureQueue.push(std::make_pair(name, file));
}

template<>
void ResourceManager::AddInResourceQueue<Material>(const std::string& name, const MaterialData& data)
{
    materialQueue.push(std::make_pair(name, data));
}

template<>
std::shared_ptr<Texture> ResourceManager::Get<>(const std::string &name) const { return textureMap.at(name); }

template<>
std::shared_ptr<Material> ResourceManager::Get<>(const std::string &name) const { return materialMap.at(name); }
