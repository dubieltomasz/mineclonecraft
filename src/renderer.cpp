#include "../include/renderer.hpp"
#include "../include/calc.hpp"
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <cstdint>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define fov 90.0f

Renderer::Color::Color(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Renderer::Color Renderer::Color::Magenta() { return Renderer::Color(1.0f, 0.0f, 1.0f); }

Renderer::Color Renderer::Color::Cyan() { return Renderer::Color(0.0f, 1.0f, 1.0f); }

Renderer::Color Renderer::Color::Yellow() { return Renderer::Color(1.0f, 1.0f, 0.0f); }

Renderer::Color Renderer::Color::White() { return Renderer::Color(1.0f, 1.0f, 1.0f); }

Renderer::GameObject::GameObject(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Color color) {
  this->vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
  this->normal = 0;
  this->r = color.r;
  this->b = color.b;
  this->g = color.g;
}

Renderer::Triangle::Triangle() {
  this->vertices = {};
  this->indices = {};
  this->normal = {};
}

void Renderer::addGlobalVertices(std::vector<SDL_Vertex>& v, const std::vector<std::tuple<float, float, float>> &vertices, const calc::Mat4& camera , float r, float b, float g) {
  for (const auto &[x, y, z] : vertices) {
    calc::Vec4 vertexGlobal(x, y, z, 1.0f);
    calc::Vec4 vertexCamera = camera * vertexGlobal;

    if (vertexCamera.z >= 0.0f) {
      continue;
    }

    float f = 1.0f / std::tan((fov * 0.5f) * M_PI / 180.0f);
    float dx = (vertexCamera.x) * f / -vertexCamera.z;
    float dy = (vertexCamera.y) * f / -vertexCamera.z;

    SDL_Vertex newVertex = {};
    newVertex.position.x = dx * (windowWidth * 0.5f) + windowWidth * 0.5f;
    newVertex.position.y = -dy * (windowHeight * 0.5f) + windowHeight * 0.5f;
    newVertex.color.a = 1.0f;
    newVertex.color.r = r;
    newVertex.color.b = b;
    newVertex.color.g = g;

    v.push_back(newVertex);
  }
}

Renderer::Renderer(SDL_Window* window, int r, int g, int b) {
  this->renderer = SDL_CreateRenderer(window, NULL);
  

  this->r = r;
  this->g = g;
  this->b = b;
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(this->renderer);
}

void Renderer::render(const std::string& fps, int objects, std::vector<GameObject> triangles, const Player& player) {
  SDL_SetRenderDrawColor(this->renderer, this->r, this->g, this->b, 255);
  SDL_RenderClear(this->renderer);

  std::vector<SDL_Vertex> vertices;
  vertices.reserve(triangles.size() * 3);
  
  for (const GameObject &gameObject : triangles) {
    addGlobalVertices(vertices, gameObject.vertices, player.camera, gameObject.r, gameObject.b, gameObject.g);
  }

  std::sort(
    triangles.begin(),
    triangles.end(),
    [&](const GameObject &g1, const GameObject &g2) -> bool {
      float sumX1 = 0.0f, sumY1 = 0.0f, sumZ1 = 0.0f;
      float sumX2 = 0.0f, sumY2 = 0.0f, sumZ2 = 0.0f;

      for (auto [x, y, z] : g1.vertices) {
        sumX1 += x;
        sumY1 += y;
        sumZ1 += z;
      }

      sumX1 /= 3.0f;
      sumY1 /= 3.0f;
      sumZ1 /= 3.0f;

      for (auto [x, y, z] : g2.vertices) {
        sumX2 += x;
        sumY2 += y;
        sumZ2 += z;
      }

      sumX2 /= 3.0f;
      sumY2 /= 3.0f;
      sumZ2 /= 3.0f;

      return (std::pow(player.x - sumX1, 2)
        + std::pow(player.y - sumY1, 2)
        + std::pow(player.z - sumZ1, 2)
      ) > (std::pow(player.x - sumX2, 2)
        + std::pow(player.y - sumY2, 2)
        + std::pow(player.z - sumZ2, 2)
      );
    }
  );

  SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, vertices.size());

  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDebugText(this->renderer, 10, 10, ("FPS: " + fps).c_str());
  SDL_RenderDebugText(this->renderer, 10, 20, ("Objects: " + std::to_string(objects)).c_str());

  SDL_RenderPresent(this->renderer);
}

std::vector<SDL_Vertex> toGlobal(const std::vector<std::tuple<float, float, float>>& vertices, const std::vector<std::tuple<float, float, float>>& colors, const std::vector<uint8_t>& normals, const Player& player) {
  std::vector<SDL_Vertex> result = {};
  float f = 1.0f / std::tan((fov * 0.5f) * M_PI / 180.0f);

  for (int i = 0; i < normals.size(); ++i) {
    int vi0 = i * 3, vi1 = vi0 + 1, vi2 = vi1 + 1;

    bool ok = true;
    switch(normals[i]) {
      case 1:
        if(player.y > std::get<1>(vertices[vi0])) {
          ok = false;
        }
        break;
      case 2:
        if(player.x > std::get<0>(vertices[vi0])) {
          ok = false;
        }
        break;
      case 4:
        if(player.z > std::get<2>(vertices[vi0])) {
          ok = false;
        }
        break;
      case 8:
        if(player.y < std::get<1>(vertices[vi0])) {
          ok = false;
        }
        break;
      case 16:
        if(player.x < std::get<0>(vertices[vi0])) {
          ok = false;
        }
        break;
      case 32:
        if(player.z < std::get<2>(vertices[vi0])) {
          ok = false;
        }
        break;
    }

    if(!ok) {
      continue;
    }

    calc::Vec4 vertexGlobal0(std::get<0>(vertices[vi0]), std::get<1>(vertices[vi0]), std::get<2>(vertices[vi0]), 1.0f);
    calc::Vec4 vertexCamera0 = player.camera * vertexGlobal0;
    calc::Vec4 vertexGlobal1(std::get<0>(vertices[vi1]), std::get<1>(vertices[vi1]), std::get<2>(vertices[vi1]), 1.0f);
    calc::Vec4 vertexCamera1 = player.camera * vertexGlobal1;
    calc::Vec4 vertexGlobal2(std::get<0>(vertices[vi2]), std::get<1>(vertices[vi2]), std::get<2>(vertices[vi2]), 1.0f);
    calc::Vec4 vertexCamera2 = player.camera * vertexGlobal2;

    if (vertexCamera0.z >= 0.0f || vertexCamera1.z >= 0.0f || vertexCamera2.z >= 0.0f) {
      continue;
    }

    float dx = (vertexCamera0.x) * f / -vertexCamera0.z;
    float dy = (vertexCamera0.y) * f / -vertexCamera0.z;

    SDL_Vertex newVertex = {};
    newVertex.position.x = dx * (windowWidth * 0.5f) + windowWidth * 0.5f;
    newVertex.position.y = -dy * (windowHeight * 0.5f) + windowHeight * 0.5f;
    newVertex.color.a = 1.0f;
    newVertex.color.r = std::get<0>(colors[i / 3]);
    newVertex.color.b = std::get<1>(colors[i / 3]);
    newVertex.color.g = std::get<2>(colors[i / 3]);

    result.push_back(newVertex);
    
    dx = (vertexCamera1.x) * f / -vertexCamera1.z;
    dy = (vertexCamera1.y) * f / -vertexCamera1.z;

    newVertex.position.x = dx * (windowWidth * 0.5f) + windowWidth * 0.5f;
    newVertex.position.y = -dy * (windowHeight * 0.5f) + windowHeight * 0.5f;
    newVertex.color.a = 1.0f;
    newVertex.color.r = std::get<0>(colors[i / 3]);
    newVertex.color.b = std::get<1>(colors[i / 3]);
    newVertex.color.g = std::get<2>(colors[i / 3]);

    result.push_back(newVertex);
    
    dx = (vertexCamera2.x) * f / -vertexCamera2.z;
    dy = (vertexCamera2.y) * f / -vertexCamera2.z;

    newVertex.position.x = dx * (windowWidth * 0.5f) + windowWidth * 0.5f;
    newVertex.position.y = -dy * (windowHeight * 0.5f) + windowHeight * 0.5f;
    newVertex.color.a = 1.0f;
    newVertex.color.r = std::get<0>(colors[i / 3]);
    newVertex.color.b = std::get<1>(colors[i / 3]);
    newVertex.color.g = std::get<2>(colors[i / 3]);

    result.push_back(newVertex);
  }

  return result;
}

void Renderer::renderTerrain(const std::string& fps, int objects, const std::vector<std::tuple<float, float, float>>& vertices, const std::vector<std::tuple<float, float, float>>& colors, const std::vector<uint8_t>& normals, const std::vector<int>& indices, const Player& player) {
  SDL_SetRenderDrawColor(this->renderer, this->r, this->g, this->b, 255);
  SDL_RenderClear(this->renderer);

  std::vector<SDL_Vertex> triangles = toGlobal(vertices, colors, normals, player);

  SDL_RenderGeometry(renderer, nullptr, triangles.data(), triangles.size(), nullptr, triangles.size());

  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDebugText(this->renderer, 10, 10, ("FPS: " + fps).c_str());
  SDL_RenderDebugText(this->renderer, 10, 20, ("Objects: " + std::to_string(objects)).c_str());

  SDL_RenderPresent(this->renderer);
}
