#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Renderer {
private:
  SDL_Window* window = nullptr;
  VkInstance instance = NULL;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t presentationFamilyIndices = 0;
  uint32_t graphicsFamilyIndices = 0;
  VkDevice device = NULL;
  VkPhysicalDeviceFeatures deviceFeatures{};
  VkQueue graphicsQueue = NULL;
  VkSurfaceKHR surface = NULL;
  VkQueue presentQueue = NULL;
  float queuePriority = 1.0f;
  const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  VkSwapchainKHR swapChain = NULL;
  std::vector<VkImage> swapChainImages = {};
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews = {};
  std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers = {};
  std::vector<VkSemaphore> imageAvailableSemaphores = {};
  std::vector<VkSemaphore> renderFinishedSemaphores = {};
  std::vector<VkFence> inFlightFences = {};
  VkPipeline graphicsPipeline;
  bool framebufferResized = false;

  void createInstance();
  void createSurface();
  std::vector<VkDeviceQueueCreateInfo> pickPhysicalDevice();
  void createLogicalDevice(std::vector<VkDeviceQueueCreateInfo> queueCreateInfo);
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createGraphicalPipeline();
  void createFramebuffers();
  void createCommandBuffers();

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  std::vector<char> readFile(const std::string& filename);
  VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline pipeline);
  void recreateSwapChain();
  void cleanupSwapChain();

public:
  Renderer(SDL_Window* window);
  ~Renderer();

  void drawFrame();
  void windowResized();
};
