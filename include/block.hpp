#include <cstdint>

class Block {
public:
  enum Blocks : uint8_t {
    Air = 0,
    Dirt,
    Stone
  };
};
