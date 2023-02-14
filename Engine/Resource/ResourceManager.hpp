#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Engine/Render/Font.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Model.hpp"
#include "Engine/Render/Texture.hpp"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <queue>
#include <memory>
#include <thread>

class World;

template<typename T>
struct ResourceLoadData;

template<>
struct ResourceLoadData<Material>
{
    std::string vertexShaderFile;
    std::string fragmentShaderFile;

    std::shared_ptr<Material> ptr;
};

template<>
struct ResourceLoadData<Texture>
{
    std::string file;
    int filtering;

    std::shared_ptr<Texture> ptr;
};

template<>
struct ResourceLoadData<Model>
{
    std::string file;

    std::shared_ptr<Model> ptr;
};

template<>
struct ResourceLoadData<Font>
{
    std::string file;
    float size;
    std::string mat;

    std::shared_ptr<Font> ptr;
};

class ResourceManager
{
    public:
        static ResourceManager* CreateResourceManager(GLFWwindow* window, World* world);
        ~ResourceManager();

        template<typename T>
        std::shared_ptr<T> AddInResourceQueue(const std::string& name, ResourceLoadData<T> data);
        
        void StartLoading();
        float GetLoadStatus(std::string* str) const;
        bool HasLoadingFinished() const;

        // blocking call to finish loading
        void Load();
        
        template<typename T>
        std::shared_ptr<T> Get(const std::string& name) const;

    private:
        World* world;

        // function running on a separate thread loading resources
        void Loader();

        std::thread loaderThread;
        GLFWwindow* context;

        bool finished = false;
        int totalTextures, totalMaterials, totalModels, totalFonts;
        int texturesLoaded, materialsLoaded, modelsLoaded, fontsLoaded;

        std::vector<std::pair<std::string, ResourceLoadData<Texture>>> textureQueue;
        std::vector<std::pair<std::string, ResourceLoadData<Material>>> materialQueue;
        std::vector<std::pair<std::string, ResourceLoadData<Model>>> modelQueue;
        std::vector<std::pair<std::string, ResourceLoadData<Font>>> fontQueue;

        std::unordered_map<std::string, Shader*> shaderMap;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
        std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;
        std::unordered_map<std::string, std::shared_ptr<Model>> modelMap;
        std::unordered_map<std::string, std::shared_ptr<Font>> fontMap;
};

#endif
