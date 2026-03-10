#include "../include/chunk.hpp"
#include <cstdint>
#include <tuple>

Chunk::Chunk() {
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->blocks = {};
}

Chunk::Chunk(float x, float y, float z, const std::array<terrainGeneration::Vec2, 256>& v, float scale) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->blocks = {};

  for(int chunkX = 0; chunkX < 16; ++chunkX) {
    for(int chunkZ = 0; chunkZ < 16; ++chunkZ) {
      this->blocks[chunkZ + (chunkX << 4) + (
        static_cast<int>(
          std::round(
            terrainGeneration::noise(
              static_cast<float>(chunkX) * scale,
              static_cast<float>(chunkZ) * scale,
              v
            ) * 10
          ) + 1.0f
        ) << 8)] = 1;
      /*addBlock(
        static_cast<float>(0),
        std::round(
          terrainGeneration::noise(
            static_cast<float>(0) * scale,
            static_cast<float>(0) * scale,
            v
          ) * 20.0f),
        static_cast<float>(0),
        terrain
      );*/
    }
  }
}

void Chunk::loadChunk(std::vector<std::tuple<float, float, float>>& vertices, std::vector<std::tuple<float, float, float>>& colors, std::vector<uint8_t>& normals) const {
  for(int chunkY = 0; chunkY < 16; ++chunkY) {
    for(int chunkX = 0; chunkX < 16; ++chunkX) {
      for(int chunkZ = 0; chunkZ < 16; ++chunkZ) {
        if(this->blocks[chunkZ + (chunkX << 4) + (chunkY << 8)] != 0) {
          //bot
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          colors.push_back(std::make_tuple(1.0f, 1.0f, 0.0f));
          colors.push_back(std::make_tuple(0.0f, 0.0f, 1.0f));
          normals.push_back(1);
          normals.push_back(1);

          //left
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          colors.push_back(std::make_tuple(1.0f, 0.0f, 1.0f));
          colors.push_back(std::make_tuple(0.0f, 1.0f, 0.0f));
          normals.push_back(2);
          normals.push_back(2);

          //back
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          colors.push_back(std::make_tuple(0.0f, 1.0f, 1.0f));
          colors.push_back(std::make_tuple(1.0f, 0.0f, 0.0f));
          normals.push_back(4);
          normals.push_back(4);

          //top
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          colors.push_back(std::make_tuple(1.0f, 1.0f, 0.0f));
          colors.push_back(std::make_tuple(0.0f, 0.0f, 1.0f));
          normals.push_back(8);
          normals.push_back(8);

          //right
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          colors.push_back(std::make_tuple(1.0f, 0.0f, 1.0f));
          colors.push_back(std::make_tuple(0.0f, 1.0f, 0.0f));
          normals.push_back(16);
          normals.push_back(16);

          //front
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          vertices.push_back(std::make_tuple(this->x + chunkX + 1.0f, this->y + chunkY + 1.0f, this->z + chunkZ + 1.0f));
          colors.push_back(std::make_tuple(0.0f, 1.0f, 1.0f));
          colors.push_back(std::make_tuple(1.0f, 0.0f, 0.0f));
          normals.push_back(32);
          normals.push_back(32);
        }
      }
    }
  }
}
