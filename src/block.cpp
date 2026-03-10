#include "../include/block.hpp"

Block::Block() {
  this->size = 1.0f;
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->texture = nullptr;
}

Block::Block(float size, float x, float y, float z, SDL_Texture texture) {
  this->size = 1.0f;
  this->x = x;
  this->y = y;
  this->z = z;
  this->texture = nullptr;
}

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
