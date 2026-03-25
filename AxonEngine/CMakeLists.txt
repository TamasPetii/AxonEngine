file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
    "*.hpp"
    "*.vert"
    "*.frag"
)

add_executable(AxonEngine ${ENGINE_SOURCES})

add_custom_command(TARGET AxonEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
    $<TARGET_FILE_DIR:AxonEngine>/Shaders
)

target_link_libraries(AxonEngine PRIVATE
    glm::glm
    GLEW::GLEW
    glfw
    assimp::assimp
    EnTT::EnTT
    imgui::imgui
)

target_include_directories(AxonEngine PRIVATE ${Stb_INCLUDE_DIR})