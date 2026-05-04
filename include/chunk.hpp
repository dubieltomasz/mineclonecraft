#pragma once

#include "calc.hpp"
#include "block.hpp"
#include <array>
#include <SDL3/SDL.h>

class Chunk {
public:
  int x, y, z;
  std::array<Block::Type, 16 * 16 * 16> blocks;

  Chunk();
  Chunk(int x, int y, int z, const std::array<calc::Vec2, 256>& v, float scale);
};
