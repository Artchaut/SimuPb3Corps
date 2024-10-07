file(GLOB SOURCES LIST_DIRECTORIES true *.cpp)
file(GLOB HEADERS LIST_DIRECTORIES true *.h)

#target_sources(imgui PRIVATE ${SOURCES} PUBLIC ${HEADERS})

#target_include_directories(imgui INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_library(imgui ${SOURCES} ${HEADERS})
add_library(imgui::imgui ALIAS imgui)
