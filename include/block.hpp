#include <cstdint>

class Block {
public:
  static constexpr float blockSize = 0.5f;
  enum Blocks : uint8_t {
    Air = 0,
    Dirt = 1,
    Stone = 2
  };
};
