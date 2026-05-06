#pragma once

#include <SDL3/SDL_vulkan.h>
#include <cstdint>
#include <vulkan/vulkan.h>
#include "vertex.hpp"
#include "chunk.hpp"
#include <vector>

class Terrain {
private:
  void gridyMesher(const std::vector<Chunk>& chunks);

public:
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  Terrain();
};
