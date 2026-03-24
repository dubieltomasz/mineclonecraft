#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "player.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct Tri {
  int ind1, ind2, ind3;
  float dist;
};

class Surface {
public:
  float x, y, z;
  uint32_t texCorAndNormal;

  inline uint8_t getTextureIndex() const {
    return this->texCorAndNormal >> 24;
  }

  inline uint8_t getAO() const {
    return static_cast<uint8_t>((this->texCorAndNormal & 0b11111111000) >> 3);
  }

  inline uint8_t getNormal() const {
    return this->texCorAndNormal & 0b111;
  }

  inline float centerX() const {
    switch(this->getNormal()) {
      case 0b000:
        return this->x + 0.5f;
      case 0b001:
        return this->x;
      case 0b010:
        return this->x + 0.5f;
      case 0b100:
        return this->x + 0.5f;
      case 0b101:
        return this->x + 1.0f;
      default:
        return this->x + 0.5f;
    }
  }

  inline float centerY() const {
    switch(this->getNormal()) {
      case 0b000:
        return this->x;
      case 0b001:
        return this->x + 0.5f;
      case 0b010:
        return this->x + 0.5f;
      case 0b100:
        return this->x + 1.0f;
      case 0b101:
        return this->x + 0.5f;
      default:
        return this->x + 0.5f;
    }
  }

  inline float centerZ() const {
    switch(this->getNormal()) {
      case 0b000:
        return this->x + 0.5f;
      case 0b001:
        return this->x + 0.5f;
      case 0b010:
        return this->x;
      case 0b100:
        return this->x + 0.5f;
      case 0b101:
        return this->x + 0.5f;
      default:
        return this->x + 1.0f;
    }
  }
};

class Renderer {
public:
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  int r, g, b;

  Renderer(SDL_Window* window, int r, int g, int b);

  ~Renderer();

  bool ok();

  void makeTrianglesWithIndices(const std::vector<Surface>& surfaces, std::vector<SDL_Vertex>& triangles, std::vector<Tri>& indices, const Player& player);

  Renderer& prepare();

  Renderer& terrain(const std::vector<Surface>& surfaces, const Player& player);

  Renderer& hud(const std::vector<std::string>& strings, const std::vector<std::pair<int, int>>& positions);

  void render();

  enum surfaceDirection : uint32_t {
    FaceBottom = 0b000,
    FaceLeft,
    FaceBack,
    FaceTop = 0b100,
    FaceRight,
    FaceFront,
    SurfaceNormal = 0b111
  };

  enum AO : uint32_t {
    FullDark = 0b00,
    VeryDark = 0b01,
    LittleDark = 0b10,
    NotDark = 0b11,
    VertexAO = 0b11
  };
};
