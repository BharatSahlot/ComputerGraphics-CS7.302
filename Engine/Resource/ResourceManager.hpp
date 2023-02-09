#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "GLFW/glfw3.h"

#include <unordered_map>
#include <queue>
#include <memory>
#include <thread>

struct MaterialData
{
    std::string vertexShaderFile;
    std::string fragmentShaderFile;
};

class ResourceManager
{
    public:
        static ResourceManager* CreateResourceManager(GLFWwindow* window);
        ~ResourceManager();

        template<typename T, typename U>
        void AddInResourceQueue(const std::string& name, const U& data);
        
        void StartLoading();
        std::string GetLoadStatus() const;
        bool HasLoadingFinished() const;

        // blocking call to finish loading
        void Load();
        
        template<typename T>
        std::shared_ptr<T> Get(const std::string& name) const;

    private:
        // function running on a separate thread loading resources
        void Loader();

        std::thread loaderThread;
        GLFWwindow* context;

        int totalTextures, totalMaterials;
        int texturesLoaded, materialsLoaded;

        std::queue<std::pair<std::string, std::string>> textureQueue;
        std::queue<std::pair<std::string, MaterialData>> materialQueue;

        std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
        std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;
        std::unordered_map<std::string, Shader*> shaderMap;
};

#endif
