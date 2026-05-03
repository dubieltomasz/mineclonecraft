#pragma once

#include "calc.hpp"
#include "chunk.hpp"
#include "vertex.hpp"
#include <array>
#include <math.h>
#include <vector>

namespace terrainGeneration {
std::array<calc::Vec2, 256> vectors(uint64_t seed);

inline constexpr float smoothstep(const float& x) {
  return 6.0f * std::pow(x, 5) - 15.0f * std::pow(x, 4) + 10.0f * std::pow(x, 3);
}

float noise(float x, float y, const std::array<calc::Vec2, 256> &vectors);

class Terrain {
public:
  std::vector<float> xs;
  std::vector<float> ys;
  std::vector<float> zs;
  std::vector<float> texX;
  std::vector<float> texY;
  std::vector<uint8_t> normals;

  Terrain();

  void loadChunk(const Chunk& chunk);
  
  inline void loadVertex(int x, int y, int z);
};

void surfacesFromChunks(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::vector<Chunk> &chunks);
} // namespace terrainGeneration
