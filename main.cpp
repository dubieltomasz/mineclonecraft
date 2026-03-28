#include "./include/player.hpp"
#include "./include/terrainGeneration.hpp"
#include "./include/chunk.hpp"
#include "./include/renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <cmath>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <set>
#include <algorithm>
#include <fstream>

#define windowWidth 800
#define windowHeight 600

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window) {
if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void createSwapChain(
    VkPhysicalDevice physicalDevice,
    VkDevice device,
    SDL_Window* window,
    VkSurfaceKHR surface,
    uint32_t graphicsFamily,
    uint32_t presentFamily,
    VkSwapchainKHR& swapChain,
    std::vector<VkImage>& swapChainImages,
    VkFormat& swapChainImageFormat,
    VkExtent2D& swapChainExtent
) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};

    if (graphicsFamily != presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create swapchain\n");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}


static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not read file\n");
    }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}

VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create shader module\n");
  }

  return shaderModule;
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  bool shouldClose = false;
  Uint64 lastCheck = SDL_GetPerformanceCounter(), fpsTime = lastCheck, t, freq;
  int frames = 0;
  std::string fps;

  SDL_Window* window;
  if(!(window = SDL_CreateWindow("MineCloneCraft", windowWidth, windowHeight, SDL_WINDOW_VULKAN))) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  // TODO: Throw this to rendering class
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t presentationFamilyIndices = 0;
  uint32_t graphicsFamilyIndices = 0;
  VkDevice device;
  VkPhysicalDeviceFeatures deviceFeatures{};
  VkQueue graphicsQueue;
  VkSurfaceKHR surface;
  VkQueue presentQueue;
  float queuePriority = 1.0f;
  const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  VkSwapchainKHR  swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtend;
  std::vector<VkImageView> swapChainImageViews;

  // Create instance
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t extensionsCount;
  const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionsCount);

  createInfo.enabledExtensionCount = extensionsCount;
  createInfo.ppEnabledExtensionNames = extensions;
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    SDL_Log("Could not create instance\n");
    return 1;
  }
  //

  // TODO: Add validation layers

  // Create surface
  if(!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
    SDL_Log("Could not create surface\n");
    return 1;
  }
  //

  // Pick Physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  // FIXME: Change this "magic function"
  for(const VkPhysicalDevice& device : devices) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // TODO: Assign score to each device and chose based on highiest score
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader) {
      bool isOk = false;

      // TODO: Make a struct and more options for other families
      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for(const VkQueueFamilyProperties& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          graphicsFamilyIndices = i;

          VkBool32 presentSupport = false;
          vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

          if(presentSupport) {
            presentationFamilyIndices = i;
            isOk = true;
          }
        }

        ++i;
      }

      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for(const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
      }

      bool swapChainAdequate = false;
      if (requiredExtensions.empty()) {
          SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
      }

      if(isOk && requiredExtensions.empty() && swapChainAdequate) {
        physicalDevice = device;
        break;
      }
    }
  }

  if(physicalDevice == VK_NULL_HANDLE) {
    SDL_Log("Could not find suitable physical device\n");
    return 1;
  }

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqieQueueFamilies = {graphicsFamilyIndices, presentationFamilyIndices};

  for(uint32_t queueFamily : uniqieQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }
  //

  // Create logical device
  VkDeviceCreateInfo createInfo2{};
  createInfo2.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo2.pQueueCreateInfos = queueCreateInfos.data();
  createInfo2.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo2.pEnabledFeatures = &deviceFeatures;
  createInfo2.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo2.ppEnabledExtensionNames = deviceExtensions.data();
  createInfo2.enabledLayerCount = 0;

  if(vkCreateDevice(physicalDevice, &createInfo2, nullptr, &device) != VK_SUCCESS) {
    SDL_Log("Could not create logical device\n");
    return 1;
  }

  vkGetDeviceQueue(device, graphicsFamilyIndices, 0, &graphicsQueue);
  vkGetDeviceQueue(device, presentationFamilyIndices, 0, &presentQueue);
  //

  // SwapChain
  createSwapChain(
      physicalDevice,
      device,
      window,
      surface,
      graphicsFamilyIndices,
      presentationFamilyIndices,
      swapChain,
      swapChainImages,
      swapChainImageFormat,
      swapChainExtend
  );
  //

  // Image views
  swapChainImageViews.resize(swapChainImages.size());

  for(size_t i = 0; i < swapChainImages.size(); ++i) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
      SDL_Log("Could not create image view\n");
      return 1;
    }
  }
  //

  // Graphics pipeline
  auto vertShaderCode = readFile("shaders/vert.spv");
  auto fragShaderCode = readFile("shaders/frag.spv");

  VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device);
  VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};



  //

  if (!SDL_SetWindowRelativeMouseMode(window, true)) {
    SDL_Log("Could not enable relative mouse mode: %s", SDL_GetError());
    return 1;
  }

  Player player(0.0f, 0.0f, 10.0f);

  std::array<calc::Vec2, 256> v = terrainGeneration::vectors(67);

  constexpr float scale = 0.01f;
  std::vector<Chunk> chunks;

  for(int i = 0; i < 8; ++i) {
    for(int j = 0; j < 8; ++j) {
      chunks.push_back(Chunk(i, 0, j, v, scale));
    }
  }

  terrainGeneration::Terrain terrain = terrainGeneration::Terrain();
  for(const Chunk& chunk : chunks) {
    terrain.loadChunk(chunk);
  }

  std::vector<Surface> surfaces = {};
  terrainGeneration::surfacesFromChunks(surfaces, chunks);

  SDL_Event event;
  while (!shouldClose) {  
    t = SDL_GetPerformanceCounter();
    freq = SDL_GetPerformanceFrequency();
    ++frames;

    float dt = static_cast<float>(t - lastCheck) / freq;
    lastCheck = t;

    if(t - fpsTime >= freq) {
      fps = std::to_string(frames * freq / (t - fpsTime));
      fpsTime = t;
      frames = 0;
    }

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        shouldClose = true;
      }

      player.handleEvent(event);
    }
    player.handleInput(dt);
    player.updateCamera();
  }

  for(auto imageView : swapChainImageViews) {
    vkDestroyImageView(device, imageView, nullptr);
  }

  vkDestroyShaderModule(device, fragShaderModule, nullptr);
  vkDestroyShaderModule(device, vertShaderModule, nullptr);
  vkDestroySwapchainKHR(device, swapChain, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyDevice(device, nullptr);
  vkDestroyInstance(instance, nullptr);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
