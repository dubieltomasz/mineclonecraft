#include "../include/terrain.hpp"

#include "../include/chunk.hpp"
#include "../include/terrainGeneration.hpp"

Terrain::Terrain() {
  std::array<calc::Vec2, 256> v = terrainGeneration::vectors(67);

  constexpr float scale = 0.01f;
  std::vector<Chunk> chunks;

  for(int i = 0; i < 8; ++i) {
    for(int j = 0; j < 8; ++j) {
      chunks.push_back(Chunk(i, 0, j, v, scale));
    }
  }

  terrainGeneration::surfacesFromChunks(vertices, indices, chunks);
}
