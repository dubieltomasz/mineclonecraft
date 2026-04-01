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
private:
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
  VkExtent2D swapChainExtent;
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

  void createInstance();
  void createSurface(SDL_Window* window);
  std::vector<VkDeviceQueueCreateInfo> pickPhysicalDevice();
  void createLogicalDevice(std::vector<VkDeviceQueueCreateInfo> queueCreateInfo);
  void createSwapChain(SDL_Window* window);
  void createImageViews();
  void createRenderPass();
  void createGraphicalPipeline();
  void createFramebuffers();
  void createCommandPool();

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window);
  std::vector<char> readFile(const std::string& filename);
  VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline pipeline);
  void drawFrame(VkDevice device, VkFence inFlightFence, VkSwapchainKHR swapChain, VkSemaphore imageAvailableSemaphore);

public:
  Renderer(SDL_Window* window);
  ~Renderer();

  void frame();
};
