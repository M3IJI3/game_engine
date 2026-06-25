//
// Created by Jie Mei on 2026-06-24.
//
#include "ModelLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Logger.h"
#include <iostream>

bool ModelLoader::LoadOBJ(const std::string& filepath, MeshData& outData) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;

    if (!reader.ParseFromFile(filepath, config)) {
        if (!reader.Error().empty()) {
            Logger::Error("OBJ Load Error: " + reader.Error());
        }
        return false;
    }

    if (!reader.Warning().empty()) {
        Logger::Warning("OBJ Load Warning: " + reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    if (shapes.empty()) {
        Logger::Error("No shapes found in OBJ file");
        return false;
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // 位置
                float scale = 0.05f;
                outData.positions.push_back(glm::vec3(
                    attrib.vertices[3 * idx.vertex_index + 0] * scale,
                    attrib.vertices[3 * idx.vertex_index + 1] * scale,
                    attrib.vertices[3 * idx.vertex_index + 2] * scale
                ));

                // 纹理坐标
                if (idx.texcoord_index >= 0) {
                    outData.texcoords.push_back(glm::vec2(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                    ));
                } else {
                    outData.texcoords.push_back(glm::vec2(0.0f, 0.0f));
                }

                // 法线
                if (idx.normal_index >= 0) {
                    outData.normals.push_back(glm::vec3(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    ));
                } else {
                    outData.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                outData.indices.push_back(outData.indices.size());
            }
            index_offset += fv;
        }
    }

    Logger::Info("Loaded OBJ: " + std::to_string(outData.positions.size()) + " vertices");
    return true;
}