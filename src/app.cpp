/**
 * @file    app.cpp
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
#include "app.h"

#include "log.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace {
constexpr auto vkVersion = VK_MAKE_VERSION(1, 3, 0);
}

bool App::init()
{
    if (!initWindow()) { return false; }
    if (!initVulkan()) { return false; }
    if (!initSurface()) { return false; }
    return true;
}

void App::run()
{
    while (!glfwWindowShouldClose(m_window.get())) {
        glfwPollEvents();
    }
}

bool App::initWindow()
{
    auto maybeWindow = glfw::createWindow(glm::ivec2(1280, 720), "Vulkan Learning");
    if (!maybeWindow.has_value()) { return false; }
    m_window = std::move(maybeWindow.value());
    return true;
}

bool App::initVulkan()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
    auto loaderVersion = vk::enumerateInstanceVersion();
    if (loaderVersion < vkVersion) {
        BR_APP_ERROR("Loader does not support Vulkan 1.3!");
        return false;
    }

    vk::ApplicationInfo appInfo = {
      "Vulkan Learning",
      vkVersion,
    };

    vk::InstanceCreateInfo instanceCreateInfo = {};
    // We need WSI instance extensions here (platform-specific Swapchains).
    const auto extensions = glfw::getInstanceExtensions();
    instanceCreateInfo.setPApplicationInfo(&appInfo).setPEnabledExtensionNames(extensions);
    m_instance = vk::createInstanceUnique(instanceCreateInfo);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
    return true;
}

bool App::initSurface()
{
    auto maybeSurface = glfw::createSurface(m_window.get(), *m_instance);
    if (!maybeSurface.has_value()) { return false; }
    m_surface = std::move(maybeSurface.value());
    return true;
}
