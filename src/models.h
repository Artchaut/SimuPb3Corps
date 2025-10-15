#include <cmath>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <random>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "textures.h"

#ifndef MODELS_H

#define MODELS_H

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

constexpr uint32_t MAX_MODELS = 3;
constexpr uint32_t MAX_STARS = 1000;
constexpr uint32_t STARS_DIST = 1000;

struct StarVertex {
    glm::vec3 pos;

    static auto getStarBindingDescription() -> VkVertexInputBindingDescription
    {
        VkVertexInputBindingDescription bindingDescription;

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(StarVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static auto getStarAttributeDescriptions() -> std::array<VkVertexInputAttributeDescription, 1>
    {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(StarVertex, pos);

        return attributeDescriptions;
    }
};

class StarrySky {
    std::string starModel;

    std::vector<StarVertex> vertices;
    std::vector<uint32_t> indices;

    std::vector<VkDescriptorSet> descriptorSets;

    auto createSky() -> void
    {
    }

    auto loadStar() -> void
    {
        tinyobj::attrib_t attrib;

        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, starModel.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                StarVertex vertex {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }
    }

    static auto randomSpherePoint() -> glm::vec3
    {

        std::random_device rd; // Seed
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_real_distribution<> dis(0.0, 1.0); // Range [0, 1)

        double u = dis(gen);
        double v = dis(gen);

        double theta = glm::pi<double>() * 2 * u;
        double phi = acos(2 * v - 1);

        return glm::vec3(STARS_DIST * sin(theta) * cos(phi), STARS_DIST * sin(theta) * sin(phi), STARS_DIST * cos(phi));
    }
};

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

class ModelData {
public:
    std::string modelPath;
    std::string texturePath;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float rotationDeg;

    glm::vec3 position;
    glm::vec3 scaleFactor = { 1.0f, 1.0f, 1.0f };
    glm::vec3 rotationAxis;

    std::vector<VkBuffer> uniformBuffers;

    std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::vector<void*> uniformBuffersMapped;

    std::vector<VkDescriptorSet> descriptorSets;

    ModelData(std::string mPath,
        std::string tPath,
        float rotationDeg,
        glm::vec3 scaleFactor,
        glm::vec3 rotationAxis)
        : modelPath(mPath)
        , texturePath(tPath)
        , rotationDeg(rotationDeg)
        , rotationAxis(rotationAxis)
        , scaleFactor(scaleFactor)
    {
        loadModel();
    }

    ModelData(std::string mPath,
        std::string tPath)
        : modelPath(mPath)
        , texturePath(tPath)
        , rotationDeg(0.0f)
        , rotationAxis(glm::vec3(0.0f, 1.0f, 0.0f))
        , scaleFactor(glm::vec3(1.0f, 1.0f, 1.0f))
    {

        loadModel();
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

class Planet {
public:
    float m_Mass;

    glm::vec3 m_PositionInit;
    glm::vec3 m_Position;

    glm::vec3 m_VelocityInit;
    glm::vec3 m_Velocity;

    ModelData m_Model;
    Texture m_Texture;

    Planet(float mass, ModelData model, Texture texture, glm::vec3 position_init, glm::vec3 velocity_init)
        : m_Mass(mass)
        , m_Position(position_init)
        , m_PositionInit(position_init)
        , m_Velocity(velocity_init)
        , m_VelocityInit(velocity_init)
        , m_Model(model)
        , m_Texture(texture)
    {
    }

    auto reset() -> void
    {
        m_Position = m_PositionInit;
        m_Velocity = m_VelocityInit;
    }

    auto update(float time_elapsed, const std::array<Planet, MAX_MODELS>& other_planets) -> void
    {
        for (auto& other : other_planets) {
            if (&other != this) {

                m_Velocity += time_elapsed * other.m_Mass * std::powf(glm::distance(other.m_Position, m_Position), -3) * (other.m_Position - m_Position);
            }
        }

        m_Position += time_elapsed * m_Velocity;

        m_Model.position = m_Position;
    }
};

#endif // !MODELS_H
