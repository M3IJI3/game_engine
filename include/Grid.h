#pragma once

#include <glm/glm.hpp>
#include <vector>

class Grid {
public:
    Grid() = default;
    void Generate(int size, int subdivisions);
    void Draw(unsigned int shaderProgram, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const;
    void DrawDepth(unsigned int shaderProgram, const glm::mat4& lightSpaceMatrix) const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    int m_vertexCount = 0;
};