#include <string>

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.h>

#ifndef SHADER_H

#define SHADER_H

class Shader {
public:
    static auto getFileContent(const std::string& filePath) -> std::string;

    static auto preprocessShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source) -> std::string;

    static auto compileShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false) -> std::vector<uint32_t>;

    static auto compileShaderToAssembly(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false) -> std::string;

    static auto createShaderModule(VkDevice device, const std::vector<uint32_t>& shaderCode) -> VkShaderModule;
};

#endif
