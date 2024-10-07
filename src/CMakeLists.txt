file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

add_executable(vk_test ${SOURCES})

target_link_libraries(vk_test PUBLIC glm glfw vulkan shaderc imgui stb tinyobjloader dl pthread X11 Xxf86vm Xrandr Xi)
  
