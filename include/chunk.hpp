#pragma once

#include "terrainGeneration.hpp"
#include <array>
#include <cstdint>
#include <SDL3/SDL.h>
#include <vector>

class Chunk {
public:
  float x, y, z;
  std::array<uint8_t, 4096> blocks;

  Chunk();
  Chunk(float x, float y, float z, const std::array<terrainGeneration::Vec2, 256>& v, float scale);

  void loadChunk(std::vector<std::tuple<float, float, float>>& vertices, std::vector<std::tuple<float, float, float>>& colors, std::vector<uint8_t>& normals) const;
};
