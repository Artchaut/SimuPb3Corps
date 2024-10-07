cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_DEBUG_POSTFIX d)

project(test_vk_config LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(glm REQUIRED)

add_subdirectory(src)
add_subdirectory(lib/glfw)              #window opening
add_subdirectory(lib/glm)				#math
add_subdirectory(lib/shaderc)           #shader compiling
# add_subdirectory(test) eventually

add_subdirectory(include/imgui)
add_subdirectory(include/stb)
add_subdirectory(include/tinyobjloader)

target_include_directories(vk_test PUBLIC include/imgui)
target_include_directories(vk_test PUBLIC include/stb)
target_include_directories(vk_test PUBLIC include/tinyobjloader)

