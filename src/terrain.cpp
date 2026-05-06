#include "../include/terrain.hpp"

#include "../include/terrainGeneration.hpp"
#include <array>
#include <cstdint>
#include <unordered_set>
#include <tuple>
#include <vector>

Terrain::Terrain() {
  std::array<calc::Vec2, 256> v = terrainGeneration::vectors(67);

  constexpr float scale = 0.01f;
  std::vector<Chunk> chunks;

  for(int i = 0; i < 8; ++i) {
    for(int j = 0; j < 8; ++j) {
      chunks.push_back(Chunk(i, 0, j, v, scale));
    }
  }

  //terrainGeneration::surfacesFromChunks(vertices, indices, chunks);
  this->gridyMesher(chunks);
}

// FIX: readability
inline int foo(uint16_t i) {
  if(!i) {
    return 16;
  }

  int count = 0;
  while(!(i & 1)) {
    i >>= 1;
    ++count;
  }

  return count;
}

// TODO: refaactor
std::pair<std::vector<Vertex>, std::vector<uint32_t>> mergeSlice(std::array<uint16_t, 16>& rows, int blockType, int globX, int globY, int globZ, Color color, uint32_t& index, int direction) {
  std::vector<Vertex> vs{};
  std::vector<uint32_t> is{};

  for(int u = 0; u < 16; ++u) {
    while(rows[u] != 0) {
      int vStart = foo(rows[u]);

      uint16_t run = rows[u] >> vStart;
      int vLen = foo(~run);

      uint16_t mask = ((1 << vLen) - 1) << vStart;

      int uLen = 1;
      while (u + uLen < 16 && (rows[u + uLen] & mask) == mask)
        ++uLen;

      for (int dy = 0; dy < uLen; ++dy)
        rows[u + dy] &= ~mask;

      switch(direction) {
        case 0:
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, -1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, -1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, -1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, -1.0f, 0.0f, 0.0f}
          });
          break;
        case 1:
          vs.push_back({
            {static_cast<float>(globX), static_cast<float>(globY + u + uLen), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX), static_cast<float>(globY + u + uLen), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX), static_cast<float>(globY + u), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX), static_cast<float>(globY + u), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f, 0.0f}
          });
          break;
        case 2:
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + vStart + vLen), static_cast<float>(globZ), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + vStart + vLen), static_cast<float>(globZ), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + vStart), static_cast<float>(globZ), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + vStart), static_cast<float>(globZ), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f}
          });
          break;
        case 3:
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + 1), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + 1), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + 1), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + 1), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f}
          });
          break;
        case 4:
          vs.push_back({
            {static_cast<float>(globX + 1), static_cast<float>(globY + u + uLen), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + 1), static_cast<float>(globY + u + uLen), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + 1), static_cast<float>(globY + u), static_cast<float>(globZ + vStart + vLen), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + 1), static_cast<float>(globY + u), static_cast<float>(globZ + vStart), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
          });
          break;
        case 5:
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + vStart + vLen), static_cast<float>(globZ + 1), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + vStart + vLen), static_cast<float>(globZ + 1), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u), static_cast<float>(globY + vStart), static_cast<float>(globZ + 1), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
          });
          vs.push_back({
            {static_cast<float>(globX + u + uLen), static_cast<float>(globY + vStart), static_cast<float>(globZ + 1), 1.0f},
            {color.r, color.g, color.b, 1.0f},
            {1.0f / 4.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
          });
          break;
      }

      is.push_back(index);
      is.push_back(index + 1);
      is.push_back(index + 2);
      is.push_back(index + 1);
      is.push_back(index + 3);
      is.push_back(index + 2);

      index += 4;
    }
  }

  return {vs, is};
}

// TODO: refactor
void Terrain::gridyMesher(const std::vector<Chunk>& chunks) {
  this->vertices.clear();
  this->vertices.reserve(16*16*16*6*4);
  this->indices.clear();
  this->indices.reserve(16*16*16*6*6);
  uint32_t index = 0;
  std::unordered_set<uint64_t> us{};
  std::array<std::array<std::array<std::array<uint16_t, 16>, 16>, 6>, 4> slices{};

  for(const Chunk& chunk : chunks) {
    for(uint8_t x = 0; x < 16; ++x) {
      for(uint8_t y = 0; y < 16; ++y) {
        if(chunk.blocks[(y << 8) | (x << 4) | 0] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | ((chunk.z + 0)));
        }
        if(chunk.blocks[(y << 8) | (x << 4) | 15] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | ((chunk.z + 15)));
        }
      }
      for(uint8_t z = 0; z < 16; ++z) {
        if(chunk.blocks[(0 << 8) | (x << 4) | z] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + 0) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | ((chunk.z + z)));
        }
        if(chunk.blocks[(15 << 8) | (x << 4) | z] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + 15) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | ((chunk.z + z)));
        }
      }
    }

    for(uint8_t y = 0; y < 16; ++y) {
      for(uint8_t z = 0; z < 16; ++z) {
        if(chunk.blocks[(y << 8) | (0 << 4) | z] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + 0) << 8) | ((chunk.z + z)));
        }
        if(chunk.blocks[(y << 8) | (15 << 4) | z] != Block::Type::Air) {
          us.insert((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + 15) << 8) | ((chunk.z + z)));
        }
      }
    }
  }

  for(const Chunk& chunk : chunks) {
    slices = {};

    for(int y = 0; y < 16; ++y) {
      for(int x = 0; x < 16; ++x) {
        for(int z = 0; z < 16; ++z) {
          Block::Type block = chunk.blocks[(y << 8) | (x << 4) | z];

          if(block == Block::Type::Air) {
            continue;
          }

          if(y == 0 ? us.find((static_cast<uint64_t>(chunk.y + y - 1) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | (chunk.z + z)) == us.end() : chunk.blocks[((y - 1) << 8) | (x << 4) | z] == Block::Type::Air) {
            slices[block - 1][0][y][x] |= (1 << z);
          }
          if(x == 0 ? us.find((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x - 1) << 8) | (chunk.z + z)) == us.end() : chunk.blocks[(y << 8) | ((x - 1) << 4) | z] == Block::Type::Air) {
            slices[block - 1][1][x][y] |= (1 << z);
          }
          if(z == 0 ? us.find((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | (chunk.z + z - 1)) == us.end() : chunk.blocks[(y << 8) | (x << 4) | (z - 1)] == Block::Type::Air) {
            slices[block - 1][2][z][x] |= (1 << y);
          }
          if(y == 15 ? us.find((static_cast<uint64_t>(chunk.y + y + 1) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | (chunk.z + z)) == us.end() : chunk.blocks[((y + 1) << 8) | (x << 4) | z] == Block::Type::Air) {
            slices[block - 1][3][y][x] |= (1 << z);
          }
          if(x == 15 ? us.find((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x + 1) << 8) | (chunk.z + z)) == us.end() : chunk.blocks[(y << 8) | ((x + 1) << 4) | z] == Block::Type::Air) {
            slices[block - 1][4][x][y] |= (1 << z);
          }
          if(z == 15 ? us.find((static_cast<uint64_t>(chunk.y + y) << 16) | (static_cast<uint64_t>(chunk.x + x) << 8) | (chunk.z + z + 1)) == us.end() : chunk.blocks[(y << 8) | (x << 4) | (z + 1)] == Block::Type::Air) {
            slices[block - 1][5][z][x] |= (1 << y);
          }
        }
      }
    }

    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][0][j], i, chunk.x, chunk.y + j, chunk.z, Block::mapColor(static_cast<Block::Type>(i)), index, 0);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][1][j], i, chunk.x + j, chunk.y, chunk.z, Block::mapColor(static_cast<Block::Type>(i)), index, 1);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][2][j], i, chunk.x, chunk.y, chunk.z + j, Block::mapColor(static_cast<Block::Type>(i)), index, 2);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][3][j], i, chunk.x, chunk.y + j, chunk.z, Block::mapColor(static_cast<Block::Type>(i)), index, 3);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][4][j], i, chunk.x + j, chunk.y, chunk.z, Block::mapColor(static_cast<Block::Type>(i)), index, 4);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
      for(int j = 0; j < 16; ++j) {
        auto [e, r] = mergeSlice(slices[i][5][j], i, chunk.x, chunk.y, chunk.z + j, Block::mapColor(static_cast<Block::Type>(i)), index, 5);
        vertices.insert(vertices.end(), e.begin(), e.end());
        indices.insert(indices.end(), r.begin(), r.end());
      }
    }
  }

  this->vertices.shrink_to_fit();
  this->indices.shrink_to_fit();
}
