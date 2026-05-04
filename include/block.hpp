#pragma once

#include <cstdint>
#include <string>

class Color {
public:
  float r, g, b;

  Color(std::string hex) {
    if(hex.front() == '#') {
      hex = hex.substr(1);
    }

    if(hex.size() != 6) {
      r = g = b = 0.0f;
      return;
    }

    r = std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f;
    g = std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f;
    b = std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f;
  }
};

namespace Block {
  constexpr float blockSize = 1.0f;
  enum Type : uint8_t {
    Air = 0,
    Dirt = 1,
    Stone = 2
  };
  Color mapColor(Type type);
};
