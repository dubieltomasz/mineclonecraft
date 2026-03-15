#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "chunk.hpp"
#include "player.hpp"
#include <string>
#include <vector>

class Renderer {
public:
  class Color {
  public:
    float r, g, b;

    Color(float r, float g, float b);

    static Color Magenta();

    static Color Cyan();

    static Color Yellow();

    static Color White();
  };

  class GameObject {
  public:
    std::vector<std::tuple<float, float, float>> vertices;
    int normal;
    float r, b, g;

    GameObject(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Color color);
  };

  void addGlobalVertices(std::vector<SDL_Vertex>& v, const std::vector<std::tuple<float, float, float>> &vertices, const calc::Mat4& camera , float r, float b, float g);

  SDL_Renderer* renderer;
  int r, g, b;

  Renderer(SDL_Window* window, int r, int g, int b);

  ~Renderer();

  void render(const std::string& fps, int objects, std::vector<GameObject> triangles, const Player& player);

  void renderTerrain(const std::string& fps, int objects, const std::vector<std::tuple<float, float, float>>& vertices, const std::vector<std::tuple<float, float, float>>& colors, const std::vector<uint8_t>& normals, const Player& player);
};
