#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
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
  
  std::vector<SDL_Vertex> ProjectToScreen(const std::vector<std::tuple<float, float, float>> &vertices, float camX, float camY, float camZ, float camRotX, float camRotY, float r, float b, float g);
  
  SDL_Renderer* renderer;
  int r, g, b;
  
  Renderer(SDL_Window* window, int r, int g, int b);

  ~Renderer();

  void render(const std::string& fps, int objects, std::vector<GameObject> v, float cameraX, float cameraY, float cameraZ, float camRotX, float camRotY);
};
