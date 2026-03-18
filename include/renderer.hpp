#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "player.hpp"
#include <string>
#include <vector>

struct Surfacea {
  float x, y, z;
  uint32_t texCorAndNormal;
};

class Renderer {
public:
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  int r, g, b;

  Renderer(SDL_Window* window, int r, int g, int b);

  ~Renderer();

  bool ok();

  std::vector<SDL_Vertex> makeTrianglesWithIndices(const std::vector<Surfacea>& surfaces, std::vector<int>& indices, const Player& player);

  Renderer& prepare();

  Renderer& terrain(const std::vector<Surfacea>& surfaces, const Player& player);

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
};
