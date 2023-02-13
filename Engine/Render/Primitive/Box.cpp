#include "Primitive.hpp"

#include "Engine/World.hpp"
#include "Engine/Window/Window.hpp"

Primitive::Primitive(World* world) : world(world)
{
    mat = world->GetResourceManager().AddInResourceQueue("Primitive Material", ResourceLoadData<Material> {
        "Shaders/Primitives/base.vs",
        "Shaders/Primitives/base.fs"
    });

    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);

    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER,
            6 * sizeof(float),
            NULL,
            GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Primitive::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
    float points[] = { 
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER,
            6 * sizeof(float),
            points,
            GL_DYNAMIC_DRAW);

    mat->Use();
    mat->SetVec3("col", color);
    mat->SetUniformMat4("view", world->GetWindow().camera->View());
    mat->SetUniformMat4("proj", world->GetWindow().camera->Proj());

    glDrawArrays(GL_LINES, 0, 3);
}

void Primitive::DrawBox(glm::vec3 center, glm::vec3 extents, glm::vec3 color)
{
    DrawLine(center - extents * glm::vec3( 1,  1, 1), center - extents * glm::vec3(-1,  1, 1), color);
    DrawLine(center - extents * glm::vec3(-1,  1, 1), center - extents * glm::vec3(-1, -1, 1), color);
    DrawLine(center - extents * glm::vec3(-1, -1, 1), center - extents * glm::vec3( 1, -1, 1), color);
    DrawLine(center - extents * glm::vec3( 1, -1, 1), center - extents * glm::vec3( 1,  1, 1), color);

    DrawLine(center - extents * glm::vec3( 1,  1, -1), center - extents * glm::vec3(-1,  1, -1), color);
    DrawLine(center - extents * glm::vec3(-1,  1, -1), center - extents * glm::vec3(-1, -1, -1), color);
    DrawLine(center - extents * glm::vec3(-1, -1, -1), center - extents * glm::vec3( 1, -1, -1), color);
    DrawLine(center - extents * glm::vec3( 1, -1, -1), center - extents * glm::vec3( 1,  1, -1), color);

    DrawLine(center - extents * glm::vec3( 1,  1,  1), center - extents * glm::vec3( 1,  1, -1), color);
    DrawLine(center - extents * glm::vec3(-1,  1,  1), center - extents * glm::vec3(-1,  1, -1), color);
    DrawLine(center - extents * glm::vec3( 1, -1,  1), center - extents * glm::vec3( 1, -1, -1), color);
    DrawLine(center - extents * glm::vec3(-1, -1,  1), center - extents * glm::vec3(-1, -1, -1), color);
}
