#include <string>

#include <vulkan/vulkan_core.h>

#ifndef TEXTURES_H

#define TEXTURES_H

class Texture {
    int m_Width, m_Height, m_Channels;

    std::string m_Name;

public:
    VkImage m_Image;

    VkDeviceMemory m_ImageMemory;

    VkImageView m_ImageView;

    Texture(const std::string& name)
        : m_Name(name)
    {
    }

    auto createTextureImage(VkDevice device, VkQueue graphicsQueue, VkCommandPool commandBufferPool, VkPhysicalDevice physicalDevice) -> void;

    auto createImage(
        VkDevice device, VkPhysicalDevice physicalDevice,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties) -> void;

    auto hasStencilComponent(VkFormat format) -> bool;

    auto transitionImageLayout(VkDevice device, VkQueue graphicsQueue, VkCommandPool bufferCommandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) -> void;

    void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, VkCommandPool bufferCommandPool, VkBuffer buffer, uint32_t width, uint32_t height);

    inline auto createImageView(VkDevice device, VkFormat format, VkImageAspectFlags aspectFlags) -> VkImageView;

    auto createTextureImageView(VkDevice device) -> void;
};
#endif // !TEXTURE_H
