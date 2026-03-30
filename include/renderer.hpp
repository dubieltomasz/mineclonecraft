#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Renderer {
public:
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
  std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;
  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;
  VkFence inFlightFence;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;

  Renderer(SDL_Window* window);

  ~Renderer();

  void drawFrame(VkDevice device, VkFence inFlightFence, VkSwapchainKHR swapChain, VkSemaphore imageAvailableSemaphore);

  static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window);
  static void createSwapChain(
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
  );
  static std::vector<char> readFile(const std::string& filename);
  static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
  static void recordCommandBuffer(
    VkCommandBuffer commandBuffer,
    uint32_t imageIndex,
    VkRenderPass renderPass,
    VkFramebuffer framebuffer,
    VkExtent2D extent,
    VkPipeline pipeline
  );

  void frame();
};
