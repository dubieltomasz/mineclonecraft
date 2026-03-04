#include <array>
#include <cstdint>
#include <numbers>
#include <random>

namespace terrainGeneration {
// https://www.youtube.com/watch?v=gsJHzBTPG0Y
class Vec2 {
public:
  float x, y;

  Vec2() {
    this->x = 0;
    this->y = 0;
  }

  Vec2(const float& x1, const float& y1, const float& x2, const float& y2) {
    this->x = x2 - x1;
    this->y = y2 - y1;
  }

  Vec2(float theta) { // theta in <0,1>
    theta *= 2.0 * std::numbers::pi;
    this->x = std::cos(theta);
    this->y = std::sin(theta);
  }

  float operator*(const Vec2 &v) const {
    return this->x * v.x + this->y * v.y;
  }
};

constexpr std::array<int, 256> grid = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
    180};

std::array<Vec2, 256> vectors(uint64_t seed) {
  std::mt19937 rd(seed);
  std::uniform_real_distribution<float> urd(0.0f, 1.0f);
  std::array<Vec2, 256> vectors;

  for (int i = 0; i < 256; ++i) {
    vectors[i] = Vec2(urd(rd));
  }

  return vectors;
}

inline constexpr float smoothstep(const float& x) {
  return 6.0f * std::pow(x, 5) - 15.0f * std::pow(x, 4) + 10.0f * std::pow(x, 3);
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
