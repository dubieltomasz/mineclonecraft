#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <vulkan/vulkan_core.h>
#include "player.hpp"
#include "vertex.hpp"
#include "terrain.hpp"

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Renderer {
private:
  SDL_Window* window = nullptr;
  bool currentFrame = 0;
  VkInstance instance = NULL;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t presentationFamilyIndices = 0;
  uint32_t graphicsFamilyIndices = 0;
  VkDevice device = NULL;
  VkPhysicalDeviceProperties deviceProperties{};
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
  VkDescriptorSetLayout descriptorSetLayout;
  std::vector<std::vector<VkPipelineLayout>> pipelineLayout = {{}, {}};
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers = {};
  std::vector<VkSemaphore> imageAvailableSemaphores = {};
  std::vector<VkSemaphore> renderFinishedSemaphores = {};
  std::vector<VkFence> inFlightFences = {};
  std::vector<std::vector<VkPipeline>> graphicsPipeline = {{}, {}};
  bool framebufferResized = false;
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  std::vector<void*> uniformBuffersMapped;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;
  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;
  VkSampleCountFlagBits msaaSamples;
  VkImage colorImage;
  VkDeviceMemory colorImageMemory;
  VkImageView colorImageView;

  void createInstance();
  void createSurface();
  std::vector<VkDeviceQueueCreateInfo> pickPhysicalDevice();
  void createLogicalDevice(std::vector<VkDeviceQueueCreateInfo> queueCreateInfo);
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicalPipeline();
  void createCommandPool();
  void createColorResource();
  void createDepthResources();
  void createFramebuffers();
  void createTextureImage();
  void createTextureImageView();
  void createTextureSampler();
  void createUniformBuffers();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCommandBuffers();
  void createSyncObjects();

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  std::vector<char> readFile(const std::string& filename);
  VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D extent, std::vector<VkPipelineLayout> pipelineLayout, std::vector<VkPipeline> pipeline, Terrain& terrain);
  void recreateSwapChain();
  void cleanupSwapChain();
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
  void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  void updateUniformBuffer(bool currentFrame, Player* player);
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
  VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
  VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  VkFormat findDepthFormat();
  bool hasStencilComponent(VkFormat format);
  VkSampleCountFlagBits getMaxUsableSampleCount();
  std::pair<VkPipelineLayout, VkPipeline> createPipeline(const std::string& vertShader, const std::string& fragShader, const VkPolygonMode& polygonMode);

public:
  Renderer(SDL_Window* window);
  ~Renderer();

  void drawFrame(Player* player, Terrain& terrain);
  void windowResized();
  void createVertexBuffer(const std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
  void createIndexBuffer(const std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
};
