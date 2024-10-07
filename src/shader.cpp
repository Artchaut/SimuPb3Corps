#include "shader.h"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <shaderc/shaderc.hpp>

auto Shader::getFileContent(const std::string& filePath) -> std::string
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);

        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure err) {
        std::cerr << "Error opening file " << filePath << std::endl;
        exit(-1);
    }
    return shaderCode;
}

auto Shader::preprocessShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source) -> std::string
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source, kind, source_name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        throw std::runtime_error(std::format("Failed to preprocess shader. Error: {}", result.GetErrorMessage()));
    }

    return { result.cbegin(), result.cend() };
}

auto Shader::compileShader(const std::string& source_name,
    shaderc_shader_kind kind,
    const std::string& source,
    bool optimize) -> std::vector<uint32_t>
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions compileOptions;

    if (optimize) {
        compileOptions.SetOptimizationLevel(shaderc_optimization_level_size);
    }

    shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, kind, source_name.c_str(), compileOptions);
    if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
        throw std::runtime_error(std::format("Failed to compile module. Error: {}", module.GetErrorMessage()));
    }

    std::vector<uint32_t> spirv(module.cbegin(), module.cend());

    return spirv;
}

auto Shader::compileShaderToAssembly(const std::string& source_name,
    shaderc_shader_kind kind,
    const std::string& source,
    bool optimize) -> std::string
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    if (optimize) {
        options.SetOptimizationLevel(shaderc_optimization_level_size);
    }

    shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(
        source, kind, source_name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        throw std::runtime_error(std::format("Failed to compile module. Error: {}", result.GetErrorMessage()));
    }

    return { result.cbegin(), result.cend() };
}

auto Shader::createShaderModule(VkDevice device, const std::vector<uint32_t>& shaderCode) -> VkShaderModule
{
    VkShaderModuleCreateInfo createInfo {};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size() * sizeof(shaderCode[0]);
    createInfo.pCode = shaderCode.data();

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }

    return shaderModule;
}
