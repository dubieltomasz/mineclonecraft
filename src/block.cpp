#include "../include/block.hpp"

void addBlock(float x, float y, float z, std::vector<Renderer::GameObject> &gameObjects) {
  // top
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Renderer::Color::Cyan()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Renderer::Color::White()});
  // back
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                        Renderer::Color::Magenta()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Renderer::Color::Cyan()});
  // right
  gameObjects.push_back({x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Renderer::Color::White()});
  gameObjects.push_back({x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Renderer::Color::Yellow()});
  // bot
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Renderer::Color::Yellow()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Renderer::Color::White()});
  // front
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                        Renderer::Color::Cyan()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Renderer::Color::Magenta()});
  // left
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Renderer::Color::Yellow()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Renderer::Color::White()});
}
