#include "../include/terrainGeneration.hpp"
#include <random>
#include <math.h>

namespace terrainGeneration {
// https://www.youtube.com/watch?v=gsJHzBTPG0Y
Vec2::Vec2() {
  this->x = 0;
  this->y = 0;
}

Vec2::Vec2(const float& x1, const float& y1, const float& x2, const float& y2) {
  this->x = x2 - x1;
  this->y = y2 - y1;
}

Vec2::Vec2(float theta) { // theta in <0,1>
  theta *= 2.0 * M_PI;
  this->x = std::cos(theta);
  this->y = std::sin(theta);
}

float Vec2::operator*(const Vec2 &v) const {
  return this->x * v.x + this->y * v.y;
}

std::array<Vec2, 256> vectors(uint64_t seed) {
  std::mt19937 rd(seed);
  std::uniform_real_distribution<float> urd(0.0f, 1.0f);
  std::array<Vec2, 256> vectors;

  for (int i = 0; i < 256; ++i) {
    vectors[i] = Vec2(urd(rd));
  }

  return vectors;
}

float noise(float x, float y, const std::array<Vec2, 256> &vectors) {
  // https://www.youtube.com/watch?v=DxUY42r_6Cg
  int x0 = std::floor(x), x1 = x0 + 1;
  int y0 = std::floor(y), y1 = y0 + 1;

  Vec2 g1 = vectors[grid[(grid[x0 & 255] + y0) & 255]];
  Vec2 g2 = vectors[grid[(grid[x1 & 255] + y0) & 255]];
  Vec2 g3 = vectors[grid[(grid[x0 & 255] + y1) & 255]];
  Vec2 g4 = vectors[grid[(grid[x1 & 255] + y1) & 255]];

  Vec2 d1(x0, y0, x, y);
  Vec2 d2(x1, y0, x, y);
  Vec2 d3(x0, y1, x, y);
  Vec2 d4(x1, y1, x, y);

  float v = smoothstep(x - x0);

  return std::lerp(
    std::lerp((g1 * d1), (g2 * d2), v),
    std::lerp((g3 * d3), (g4 * d4), v),
    smoothstep(y - y0)
  );
}
} // namespace terrainGeneration
