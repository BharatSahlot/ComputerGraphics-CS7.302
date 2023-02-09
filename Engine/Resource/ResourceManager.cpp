#include "ResourceManager.hpp"
#include "Engine/Render/Material.hpp"
#include "GLFW/glfw3.h"
#include <cstring>
#include <iostream>
#include <sstream>

ResourceManager* ResourceManager::CreateResourceManager(GLFWwindow* window, World* world)
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
    manager->world = world;
    return manager;
}

ResourceManager::~ResourceManager()
{
    if(context) glfwDestroyWindow(context);
}

void ResourceManager::StartLoading()
{
    modelsLoaded = texturesLoaded = materialsLoaded = 0;
    loaderThread = std::thread([&]() {
        Loader();
    });
}

std::string ResourceManager::GetLoadStatus() const
{
    std::stringstream ss;
    if(modelsLoaded < totalModels)
    {
        ss << "Loading models (" << modelsLoaded << "/" << totalModels << ")";
        return ss.str();
    }

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

bool ResourceManager::HasLoadingFinished() const { return finished; }

void ResourceManager::Load()
{
    loaderThread.join();
    for(auto& [name, model] : modelMap)
    {
        model->Setup();
    }
}

void ResourceManager::Loader()
{
    glfwMakeContextCurrent(context);

    totalModels = modelQueue.size();
    while(!modelQueue.empty())
    {
        auto [name, data] = modelQueue.front();
        modelQueue.pop();

        if(modelMap.count(name))
        {
            modelsLoaded++;
            continue;
        }

        data.ptr->Load(data.file);
        modelMap[name] = data.ptr;
        modelsLoaded++;
    }

    totalTextures = textureQueue.size();
    totalMaterials = materialQueue.size();
    while(!textureQueue.empty())
    {
        auto [name, data] = textureQueue.front();
        textureQueue.pop();

        if(textureMap.count(name))
        {
            texturesLoaded++;
            continue;
        }

        data.ptr->Load(data.file, data.filtering);
        textureMap[name] = data.ptr;
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

        data.ptr->Load(shaderMap.at(data.vertexShaderFile), shaderMap.at(data.fragmentShaderFile));
        materialMap[name] = data.ptr;
        materialsLoaded++;
    }
    // flush the command queue
    glFinish();
    finished = true;
}

template<>
std::shared_ptr<Texture> ResourceManager::AddInResourceQueue<Texture>(const std::string& name, ResourceLoadData<Texture> data)
{
    std::cout << "to load texture " << name << std::endl;
    std::shared_ptr<Texture> ptr(new Texture);
    data.ptr = ptr;
    textureQueue.push(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Material> ResourceManager::AddInResourceQueue<Material>(const std::string& name, ResourceLoadData<Material> data)
{
    std::shared_ptr<Material> ptr(new Material);
    data.ptr = ptr;
    materialQueue.push(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Model> ResourceManager::AddInResourceQueue<Model>(const std::string& name, ResourceLoadData<Model> data)
{
    std::shared_ptr<Model> ptr(new Model(world));
    data.ptr = ptr;
    modelQueue.push(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Texture> ResourceManager::Get<>(const std::string &name) const { return textureMap.at(name); }

template<>
std::shared_ptr<Material> ResourceManager::Get<>(const std::string &name) const { return materialMap.at(name); }

template<>
std::shared_ptr<Model> ResourceManager::Get<>(const std::string &name) const { return modelMap.at(name); }
