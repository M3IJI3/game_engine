#include "Grid.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void Grid::Generate(int size, int subdivisions) {
    std::vector<glm::vec3> vertices;
    float half = size / 2.0f;
    float step = size / (float)subdivisions;

    for (int i = 0; i <= subdivisions; i++) {
        float z = -half + i * step;
        vertices.push_back(glm::vec3(-half, 0.0f, z));
        vertices.push_back(glm::vec3(half, 0.0f, z));
    }

    for (int i = 0; i <= subdivisions; i++) {
        float x = -half + i * step;
        vertices.push_back(glm::vec3(x, 0.0f, -half));
        vertices.push_back(glm::vec3(x, 0.0f, half));
    }

    m_vertexCount = vertices.size();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
}

void Grid::Draw(unsigned int shaderProgram, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const {
    if (m_vertexCount == 0) return;

    glUseProgram(shaderProgram);

    // 网格跟随摄像机水平位置
    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPos.x, 0.0f, cameraPos.z));
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shaderProgram, "uColor"), 1, glm::value_ptr(glm::vec3(0.3f, 0.8f, 0.3f)));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, m_vertexCount);
}