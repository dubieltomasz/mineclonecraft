#include "../include/chunk.hpp"
#include "../include/terrainGeneration.hpp"
#include <cstdint>
#include <tuple>
#include <utility>

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
      this->blocks[chunkZ + (chunkX << 4) + (
        static_cast<int>(
          std::round(
            (terrainGeneration::noise(
              (this->x + static_cast<float>(chunkX)) * scale,
              (this->z + static_cast<float>(chunkZ)) * scale,
              v
          ) + 1.0f ) * 0.5f * 15.0f
        )
      ) << 8)] = 1;
    }
  }
}

void Chunk::loadChunk(std::vector<std::tuple<float, float, float>>& vertices, std::vector<std::pair<float, float>>& textures, std::vector<uint8_t>& normals) const {
  for(int chunkY = 0; chunkY < 16; ++chunkY) {
    for(int chunkX = 0; chunkX < 16; ++chunkX) {
      for(int chunkZ = 0; chunkZ < 16; ++chunkZ) {
        if(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)] != 0) {
          //bot
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(1);

          //left
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(2);

          //back
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(4);

          //top
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(8);

          //right
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(16);

          //front
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          textures.push_back(std::make_pair(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)], 1.0f));
          normals.push_back(32);
        }
      }
    }
  }
}
