/**
 * @file    window.h
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


#ifndef LEARNING_WINDOW_H
#define LEARNING_WINDOW_H
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <span>
#include <string_view>

namespace glfw {
struct Deleter {
    void operator()(GLFWwindow* window) const noexcept;
};

using Window = std::unique_ptr<GLFWwindow, Deleter>;

std::optional<Window>               createWindow(glm::ivec2 size, std::string_view title);
std::optional<vk::UniqueSurfaceKHR> createSurface(GLFWwindow* window, vk::Instance instance);
std::span<const char* const>        getInstanceExtensions();
}    // namespace glfw
#endif    // LEARNING_WINDOW_H
