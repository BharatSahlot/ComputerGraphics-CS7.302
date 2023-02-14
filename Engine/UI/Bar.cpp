#include "Bar.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/World.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include <iostream>

Bar::Bar(World* world, std::string name, Anchor anchor, float width, float height) : UIObject(world, name), 
    col(glm::vec3(1, 1, 1)),
    anchor(anchor),
    width(width),
    height(height),
    fillAmount(0)
{
    mat = world->GetResourceManager().Get<Material>("barMat");
    if(!mat)
    {
        std::cerr << "Bar material not found" << std::endl;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Bar::Render()
{
    glm::vec2 screenDims(world->GetWindow().Width(), world->GetWindow().Height());
    glm::vec2 pos = CalculatePosition(anchor, glm::vec2(width * screenDims.x, height));

    glm::mat4 proj = glm::ortho(0.f, screenDims.x, 0.f, screenDims.y);

    float xpos = pos.x, ypos = pos.y;
    float w = width * screenDims.x, h = height;

    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
    };

    mat->Use();
    mat->SetFloat("fillAmount", fillAmount);
    mat->SetUniformMat4("proj", proj);
    mat->SetVec3("col", col);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
