#pragma once

#include "calc.hpp"
#include <array>
#include <math.h>
#include <cstdint>

namespace terrainGeneration {
std::array<calc::Vec2, 256> vectors(uint64_t seed);

inline constexpr float smoothstep(const float& x) {
  return 6.0f * std::pow(x, 5) - 15.0f * std::pow(x, 4) + 10.0f * std::pow(x, 3);
}

float noise(float x, float y, const std::array<calc::Vec2, 256> &vectors);
} // namespace terrainGeneration
