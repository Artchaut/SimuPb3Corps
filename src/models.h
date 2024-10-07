#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <vulkan/vulkan.h>

#ifndef MODELS_H

#define MODELS_H

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static auto getBindingDescription() -> VkVertexInputBindingDescription
    {
        VkVertexInputBindingDescription bindingDescription;

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static auto getAttributeDescriptions() -> std::array<VkVertexInputAttributeDescription, 3>
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

struct ModelData {
    std::string modelPath;
    std::string texturePath;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float rotationDeg;

    glm::vec3 scaleFactor;
    glm::vec3 rotationAxis;

    glm::vec3 position;

    ModelData(std::string mPath,
        std::string tPath,
        float rotationDeg,
        glm::vec3 scaleFactor,
        glm::vec3 rotationAxis,
        glm::vec3 position

        )
        : modelPath(mPath)
        , texturePath(tPath)
        , rotationDeg(rotationDeg)
        , rotationAxis(rotationAxis)
        , scaleFactor(scaleFactor)
        , position(position)
    {
    }

    ModelData(std::string mPath,
        std::string tPath)
        : modelPath(mPath)
        , texturePath(tPath)
        , rotationDeg(0.0f)
        , rotationAxis(glm::vec3(0.0f, 1.0f, 0.0f))
        , scaleFactor(glm::vec3(1.0f, 1.0f, 1.0f))
        , position(glm::vec3(0.0f, 0.0f, 0.0f))
    {
    }

    ModelData(std::string mPath,
        std::string tPath,
        glm::vec3 position)
        : modelPath(mPath)
        , texturePath(tPath)
        , rotationDeg(0.0f)
        , rotationAxis(glm::vec3(0.0f, 1.0f, 0.0f))
        , scaleFactor(glm::vec3(1.0f, 1.0f, 1.0f))
        , position(position)
    {
    }

    auto getModelMatrix() -> glm::mat4
    {
        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotationDeg), rotationAxis);
        m = glm::scale(m, scaleFactor);

        return m;
    }

    auto getModelMatrixWithTime(float time) -> glm::mat4
    {
        glm::mat4 m = glm::mat4(1.0f);

        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotationDeg) * time, rotationAxis);
        m = glm::scale(m, scaleFactor);

        return m;
    }

    auto loadModel() -> void
    {
        tinyobj::attrib_t attrib;

        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }
    }
};

#endif // !MODELS_H
