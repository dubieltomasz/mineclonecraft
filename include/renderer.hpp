#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "player.hpp"
#include <string>
#include <vector>

class Renderer {
public:
  void addGlobalVertices(std::vector<SDL_Vertex>& v, const std::vector<std::tuple<float, float, float>> &vertices, const calc::Mat4& camera , float r, float b, float g);

  SDL_Renderer* renderer;
  int r, g, b;

  Renderer(SDL_Window* window, int r, int g, int b);

  ~Renderer();

  bool ok();

  Renderer& prepare();

  Renderer& terrain(const std::vector<std::tuple<float, float, float>>& vertices, const std::vector<std::pair<float, float>>& texCoords, const std::vector<uint8_t>& normals, const Player& player);

  Renderer& hud(const std::vector<std::string>& strings, const std::vector<std::pair<int, int>>& positions);

  void render();
};
