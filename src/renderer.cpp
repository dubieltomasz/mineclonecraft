#include "../include/renderer.hpp"
#include "../include/calc.hpp"
#include <algorithm>

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

std::vector<SDL_Vertex> Renderer::ProjectToScreen(const std::vector<std::tuple<float, float, float>> &vertices, float camX, float camY, float camZ, float camRotX, float camRotY, float r, float b, float g) {
  std::vector<SDL_Vertex> result;

  for (const auto &[x, y, z] : vertices) {
    calc::Vec4 vertexGlobal(x, y, z, 1.0f);
    calc::Mat4 rotationX = calc::Mat4::MRotationX(camRotY * M_PI / 180.0f);
    calc::Mat4 rotationY = calc::Mat4::MRotationY(camRotX * M_PI / 180.0f);
    calc::Mat4 translation = calc::Mat4::MIdentity();
    translation(0, 3) = -camX;
    translation(1, 3) = -camY;
    translation(2, 3) = -camZ;
    calc::Mat4 rotation = rotationY * rotationX;
    calc::Mat4 viewMatrix = rotation.transpose() * translation;
    calc::Vec4 vertexCamera = viewMatrix * vertexGlobal;

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

    result.push_back(newVertex);
  }

  return result;
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

void Renderer::render(const std::string& fps, int objects, std::vector<GameObject> v, float cameraX, float cameraY, float cameraZ, float camRotX, float camRotY) {
  SDL_SetRenderDrawColor(this->renderer, this->r, this->g, this->b, 255);
  SDL_RenderClear(this->renderer);

  std::sort(
    v.begin(),
    v.end(),
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

      return std::sqrt(
        std::pow(cameraX - sumX1, 2)
        + std::pow(cameraY - sumY1, 2)
        + std::pow(cameraZ - sumZ1, 2)
      ) > std::sqrt(std::pow(cameraX - sumX2, 2)
        + std::pow(cameraY - sumY2, 2)
        + std::pow(cameraZ - sumZ2, 2)
      );
    }
  );

  for (const GameObject &gameObject : v) {
    auto tmp = ProjectToScreen(gameObject.vertices, cameraX, cameraY, cameraZ, camRotX, camRotY, gameObject.r, gameObject.b, gameObject.g);
    SDL_RenderGeometry(renderer, nullptr, tmp.data(), tmp.size(), nullptr, tmp.size());
  }

  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDebugText(this->renderer, 10, 10, ("FPS: " + fps).c_str());
  SDL_RenderDebugText(this->renderer, 10, 20, ("Objects: " + std::to_string(objects)).c_str());
  
  SDL_RenderPresent(this->renderer);
}
