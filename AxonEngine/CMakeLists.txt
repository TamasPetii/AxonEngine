file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
    "*.hpp"
    "*.vert"
    "*.frag"
)

add_executable(AxonEngine ${ENGINE_SOURCES})

target_link_libraries(AxonEngine PRIVATE
    glm::glm
    GLEW::GLEW
    glfw
    assimp::assimp
    EnTT::EnTT
    imgui::imgui
)

target_include_directories(AxonEngine PRIVATE ${Stb_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    AXN_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../"
)