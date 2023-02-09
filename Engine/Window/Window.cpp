#include "Window.hpp"

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "GLFW/glfw3.h"
#include "Globals.hpp"
#include "glm/fwd.hpp"
#include <glad/glad.h>

#include <iostream>

float windowFade;

static std::map<GLFWwindow*, Window*> glfwToWindow;

Window* Window::Create(int width, int height, const char* title)
{
    Window* window = new Window;

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    window->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window->glfwWindow == NULL)
    {
        std::cerr << "Window error" << std::endl;
        delete(window);
        return nullptr;
    }

    window->width = width;
    window->height = height;

    glfwToWindow[window->glfwWindow] = window;

    glfwMakeContextCurrent(window->glfwWindow);
    glfwSetFramebufferSizeCallback(window->glfwWindow, Window::FramebufferSizeCallback);
    glfwSwapInterval(1);
    return window;
}

int Window::Init()
{
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for(int i = 0; i < 2; i++)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);

        this->fbColBuffers[i] = texture;
    }

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "framebuffer not complete" << std::endl;
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenFramebuffers(2, ppFbos);
    glGenTextures(2, ppBufs);
    for(int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ppFbos[i]);
        glBindTexture(GL_TEXTURE_2D, ppBufs[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ppBufs[i], 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "framebuffer not complete" << std::endl;
            return -1;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->framebuffer = fbo;
    this->rbo = rbo;
    return 0;
}

void Window::SetCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
    this->camera->SetPerspective(60.f, (float)this->width / this->height);
}

void Window::SetRenderCallback(RenderCallback callback)
{
    this->cb = callback;
}

Window::~Window()
{
    glfwDestroyWindow(this->glfwWindow);
}

void Window::MakeCurrent()
{
    glfwMakeContextCurrent(this->glfwWindow);
}

void Window::Render(World* world)
{
    if(!this->cb) return;

    glViewport(0, 0, this->width, this->height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Mesh* mesh = new Mesh(std::vector<float>({
        1, 1, 0, 1, 1,
        1, -1, 0, 1, 0,
        -1, -1, 0, 0, 0,
        -1, 1, 0, 0, 1
    }), std::vector<unsigned int>({
        0, 1, 2,
        0, 3, 2
    }));

    Shader* vs = Shader::MakeShader("Shaders/fb.vs", GL_VERTEX_SHADER);
    Shader* fs = Shader::MakeShader("Shaders/fb.fs", GL_FRAGMENT_SHADER);
    Shader* blurFs = Shader::MakeShader("Shaders/blur.fs", GL_FRAGMENT_SHADER);

    std::shared_ptr<Material> mat(Material::MakeMaterial(vs, fs));
    std::shared_ptr<Material> blurMat(Material::MakeMaterial(vs, blurFs));

    std::shared_ptr<Mesh> msh(mesh);

    Object screen("screen", msh, mat);
    Object blurQ("blurQ", msh, blurMat);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    while(!glfwWindowShouldClose(this->glfwWindow))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glEnable(GL_DEPTH_TEST);

        // glViewport(0, 0, this->width, this->height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(this->cb(world))
        {
            break;
        }

        bool horizontal = true, first = true;
        unsigned int amount = 10;
        blurMat->Use();
        for(unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, ppFbos[horizontal]);
            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,
                    first ? fbColBuffers[1] : ppBufs[!horizontal]);
            blurMat->SetInt("image", 0);
            blurMat->SetInt("horizontal", horizontal);
            blurQ.Render(glm::mat4(1), glm::mat4(1));
            horizontal = !horizontal;
            first = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->fbColBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->ppBufs[!horizontal]);

        screen.material->Use();
        screen.material->SetInt("screenTexture", 0);
        screen.material->SetInt("bloomTexture", 1);
        screen.material->SetFloat("fade", windowFade);
        screen.Render(glm::mat4(1), glm::mat4(1));

        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}

float Window::Aspect() const
{
    return (float)this->width / this->height;
}

glm::vec3 Window::ViewportPointToWorld(glm::vec3 point) const
{
    glm::mat4 viewInv = glm::inverse(camera->ViewProj());
    return viewInv * glm::vec4(point, 1);
}

glm::vec3 Window::WorldToViewportPoint(glm::vec2 point) const
{
    return camera->ViewProj() * glm::vec4(point, 0, 1);
}

void Window::FramebufferSizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = glfwToWindow[win];
    window->height = height;
    window->width = width;
    if(window->camera)
    {
        window->camera->SetPerspective(60.0f, window->Aspect());
        // window->camera->SetOrthographic(width, height);
    }

    glViewport(0, 0, width, height);

    glDeleteTextures(2, window->fbColBuffers);
    glDeleteTextures(2, window->ppBufs);

    glDeleteFramebuffers(1, &window->framebuffer);
    glDeleteFramebuffers(2, window->ppFbos);

    glDeleteRenderbuffers(1, &window->rbo);

    window->Init();
}
