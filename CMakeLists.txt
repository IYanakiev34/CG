cmake_minimum_required(VERSION 3.12)

project(cgProj VERSION 1.0 LANGUAGES CXX)


set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)
set(PROJECT_NAME "test")
set(PROJECT_COMPILE_OPTIONS "-std=c++20 -g -O2 -Wall -Werror")

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILE_OPTIONS})

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew-cmake EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE libglew_static
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE "${PROJECT_SOURECE_DIR}/src"
    PRIVATE "${PROJECT_BINARY_DIR}/src")
