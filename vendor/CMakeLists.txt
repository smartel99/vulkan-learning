project(learning_vendors)

set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(glfw)

set(GLM_ENABLE_CXX_20 ON)
message(STATUS "[glm]")
add_subdirectory(glm)
target_compile_definitions(glm PUBLIC
        GLM_FORCE_XYZW_ONLY
        GLM_FORCE_RADIANS
        GLM_FORCE_DEPTH_ZERO_TO_ONE
        GLM_FORCE_SILENT_WARNINGS
        GLM_ENABLE_EXPERIMENTAL
        GLM_EXT_INCLUDED
)

message(STATUS "[Vulkan-Headers]")
add_subdirectory(vulkan-hpp)

message(STATUS "[VulkanMemoryAllocator]")
add_subdirectory(vulkan_memory_allocator)

message(STATUS "[Dear ImGui]")
add_library(imgui)
target_include_directories(imgui SYSTEM PUBLIC imgui)
target_link_libraries(imgui PUBLIC
        glfw
        Vulkan::Hpp
)
target_compile_definitions(imgui PUBLIC
    VK_NO_PROTOTYPES # Dynamically load Vulkan at runtime
)
target_sources(imgui PRIVATE
        imgui/imconfig.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        imgui/imgui.h

        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_vulkan.cpp
        imgui/backends/imgui_impl_vulkan.h
)

message(STATUS "[vma]")
add_library(vma)
target_link_libraries(vma PUBLIC
        Vulkan::Hpp
        GPUOpen::VulkanMemoryAllocator
)
target_include_directories(vma PUBLIC
        vulkan_memory_allocator/include
)
target_compile_definitions(vma PUBLIC
        VMA_STATIC_VULKAN_FUNCTIONS=0
        VMA_DYNAMIC_VULKAN_FUNCTIONS=1
)
target_sources(vma PRIVATE
        vk_mem_alloc.cpp
)

set(SPDLOG_USE_STD_FORMAT ON)
add_subdirectory(spdlog)

# ignore compiler warnings
target_compile_options(vma PRIVATE -w)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
        glm
        imgui
        vma
        spdlog
)

target_compile_definitions(${PROJECT_NAME} INTERFACE
    GLFW_INCLUDE_VULKAN
)
