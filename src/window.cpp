/**
 * @file    window.cpp
 * @author  Samuel Martel
 * @date    2025-09-19
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#include "window.h"

#include "log.h"

void Deleter::operator()(GLFWwindow *window) const noexcept
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

std::optional<Window> createWindow(glm::ivec2 size, std::string_view title)
{
    static const auto onError = [](int error, const char *description)
    { BR_APP_CRITICAL("[GLFW] Error {}: {}", error, description); };
    glfwSetErrorCallback(onError);
    if (!glfwInit())
    {
        BR_APP_CRITICAL("[GLFW] Failed to initialize");
        return std::nullopt;
    }

    // Check for vulkan support.
    if (!glfwVulkanSupported())
    {
        BR_APP_CRITICAL("[GLFW] Vulkan not supported");
        return std::nullopt;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto ret = Window{glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr), Deleter{}};
    if (!ret)
    {
        BR_APP_CRITICAL("[GLFW] Failed to create window");
        return std::nullopt;
    }

    return ret;
}
