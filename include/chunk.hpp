#pragma once

#include "calc.hpp"
#include <array>
#include <cstdint>
#include <SDL3/SDL.h>

class Chunk {
public:
  int x, y, z;
  std::array<uint8_t, 16 * 16 * 16> blocks;

  Chunk();
  Chunk(int x, int y, int z, const std::array<calc::Vec2, 256>& v, float scale);
};
