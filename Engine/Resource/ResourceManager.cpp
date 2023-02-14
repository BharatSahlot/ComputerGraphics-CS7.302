#include "ResourceManager.hpp"
#include "Engine/Render/Material.hpp"
#include "GLFW/glfw3.h"
#include <cstring>
#include <iostream>
#include <sstream>

extern void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam);

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

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } 

    manager->context = context;
    manager->world = world;
    manager->finished = false;
    manager->totalModels  = manager->totalTextures  = manager->totalMaterials  = manager->totalFonts = 0;
    manager->modelsLoaded = manager->texturesLoaded = manager->materialsLoaded = manager->fontsLoaded = 0;
    return manager;
}

ResourceManager::~ResourceManager()
{
    if(context) glfwDestroyWindow(context);
}

void ResourceManager::StartLoading()
{
    totalModels = totalTextures = totalMaterials = totalFonts = 0;
    modelsLoaded = texturesLoaded = materialsLoaded = fontsLoaded = 0;
    loaderThread = std::thread([&]() {
        Loader();
    });
}

float ResourceManager::GetLoadStatus(std::string* str) const
{
    std::stringstream ss;
    if(modelsLoaded < totalModels)
    {
        ss << "Loading models (" << modelsLoaded << "/" << totalModels << ")";
        *str = ss.str();
    } else if(texturesLoaded < totalTextures)
    {
        ss << "Loading textures (" << texturesLoaded << "/" << totalTextures << ")";
        *str = ss.str();
    } else if(materialsLoaded < totalMaterials)
    {
        ss << "Loading materials (" << materialsLoaded << "/" << totalMaterials << ")";
        *str = ss.str();
    } else if(fontsLoaded < totalFonts)
    {
        ss << "Loading fonts (" << fontsLoaded << "/" << totalFonts << ")";
        *str = ss.str();
    } else
    {
        ss << "Loading Finished";
        *str = ss.str();
    }
    return  (float)(modelsLoaded + texturesLoaded + materialsLoaded + fontsLoaded) / 
        (totalModels + totalTextures + totalMaterials + totalFonts);
}

bool ResourceManager::HasLoadingFinished() const { return finished; }

void ResourceManager::Load()
{
    loaderThread.join();
    for(auto& [name, model] : modelMap)
    {
        model->Setup();
    }

    for(auto& [name, font] : fontMap)
    {
        font->Setup();
    }
}

void ResourceManager::Loader()
{
    totalModels = totalTextures = totalMaterials = totalFonts = 0;
    glfwMakeContextCurrent(context);

    totalModels = modelQueue.size();
    totalFonts = fontQueue.size();
    while(!modelQueue.empty())
    {
        auto [name, data] = modelQueue.back();
        modelQueue.pop_back();

        if(data.ptr->Load(data.file) == -1)
        {
            std::cerr << "Unable to load model " << data.file << std::endl;
        }
        modelMap[name] = data.ptr;
        modelsLoaded++;
    }

    totalTextures = textureQueue.size();
    totalMaterials = materialQueue.size();
    while(!textureQueue.empty())
    {
        auto [name, data] = textureQueue.back();
        textureQueue.pop_back();

        if(data.ptr->Load(data.file, data.filtering) == -1)
        {
            std::cerr << "Unable to load texture " << data.file << std::endl;
        }
        textureMap[name] = data.ptr;
        texturesLoaded++;
    }

    while(!materialQueue.empty())
    {
        auto [name, data] = materialQueue.back();
        materialQueue.pop_back();

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

    while(!fontQueue.empty())
    {
        auto [name, data] = fontQueue.back();
        fontQueue.pop_back();

        if(!materialMap.count(data.mat))
        {
            std::cerr << "Material (" << data.mat << ") for font " << name << " does not exist" << std::endl;
        }

        if(data.ptr->Load(data.file, data.size, materialMap[data.mat]) == -1)
        {
            std::cerr << "Unable to load texture " << data.file << std::endl;
        }
        fontMap[name] = data.ptr;
        fontsLoaded++;
    }

    // flush the command queue
    glFinish();
    finished = true;
}

template<>
std::shared_ptr<Texture> ResourceManager::AddInResourceQueue<Texture>(const std::string& name, ResourceLoadData<Texture> data)
{
    for(auto& [tname, tdata]: textureQueue)
    {
        if(tname == name) return tdata.ptr;
    }

    std::shared_ptr<Texture> ptr(new Texture);
    data.ptr = ptr;
    textureQueue.push_back(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Material> ResourceManager::AddInResourceQueue<Material>(const std::string& name, ResourceLoadData<Material> data)
{
    for(auto& [tname, tdata]: materialQueue)
    {
        if(tname == name) return tdata.ptr;
    }

    std::shared_ptr<Material> ptr(new Material);
    data.ptr = ptr;
    materialQueue.push_back(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Model> ResourceManager::AddInResourceQueue<Model>(const std::string& name, ResourceLoadData<Model> data)
{
    for(auto& [tname, tdata]: modelQueue)
    {
        if(tname == name) return tdata.ptr;
    }

    std::shared_ptr<Model> ptr(new Model(world));
    data.ptr = ptr;
    modelQueue.push_back(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Font> ResourceManager::AddInResourceQueue<Font>(const std::string& name, ResourceLoadData<Font> data)
{
    for(auto& [tname, tdata]: fontQueue)
    {
        if(tname == name) return tdata.ptr;
    }

    std::shared_ptr<Font> ptr(new Font());
    data.ptr = ptr;
    fontQueue.push_back(std::make_pair(name, data));
    return ptr;
}

template<>
std::shared_ptr<Texture> ResourceManager::Get<>(const std::string &name) const { return textureMap.at(name); }

template<>
std::shared_ptr<Material> ResourceManager::Get<>(const std::string &name) const { return materialMap.at(name); }

template<>
std::shared_ptr<Model> ResourceManager::Get<>(const std::string &name) const { return modelMap.at(name); }

template<>
std::shared_ptr<Font> ResourceManager::Get<>(const std::string &name) const { return fontMap.at(name); }
