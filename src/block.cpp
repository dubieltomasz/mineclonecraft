#include "../include/block.hpp"

Color Block::mapColor(Type type) {
  switch(type) {
    case Air: return {"#ffffff"};
    case Dirt: return {"#a96f4c"};
    case Stone: return {"#a3a3a3"};
    default: return {"#000000"};
  }
}
