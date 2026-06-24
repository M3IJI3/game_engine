//
// Created by Jie Mei on 2026-06-24.
//
#pragma once

#include <vector>
#include <glm/glm.hpp>

struct MeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};

class ModelLoader {
public:
    static bool LoadOBJ(const std::string& filepath, MeshData& outData);
};