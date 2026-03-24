#include "../include/chunk.hpp"
#include "../include/terrainGeneration.hpp"

Chunk::Chunk() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
  this->blocks = {};
}

Chunk::Chunk(int x, int y, int z, const std::array<calc::Vec2, 256>& v, float scale) {
  this->x = x << 4;
  this->y = y << 4;
  this->z = z << 4;
  this->blocks = {};

  for(int chunkX = 0; chunkX < 16; ++chunkX) {
    for(int chunkZ = 0; chunkZ < 16; ++chunkZ) {
      int height = static_cast<int>(
          std::round(
            (terrainGeneration::noise(
              (this->x + static_cast<float>(chunkX)) * scale,
              (this->z + static_cast<float>(chunkZ)) * scale,
              v
          ) + 1.0f ) * 0.5f * 15.0f
        )
      );
      this->blocks[chunkZ + (chunkX << 4) + (height << 8)] = Block::Dirt;

      for(int chunkY = 0; chunkY < height; ++chunkY) {
        this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)] = Block::Stone;
      }
    }
  }
}
