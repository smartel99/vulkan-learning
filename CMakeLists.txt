cmake_minimum_required(VERSION 3.29)
project(learning)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_DEBUG_POSTFIX "-d")
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(vendor)

add_executable(learning main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        learning_vendors
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        <glm/glm.hpp>
        <vulkan/vulkan.hpp>
)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
)

file(GLOB_RECURSE sources LIST_DIRECTORIES false "src/*.[hc]pp")
target_sources(${PROJECT_NAME} PRIVATE
        ${sources}
)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
        $<$<NOT:$<CONFIG:Debug>>:-Werror>
)
