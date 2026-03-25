cmake_minimum_required(VERSION 3.21)
project(AxonEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(glm CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_subdirectory(AxonEngine)