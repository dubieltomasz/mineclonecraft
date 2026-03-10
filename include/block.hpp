#pragma once

#include "renderer.hpp"
#include <SDL3/SDL_render.h>
#include <vector>

class Block {
public:
  float size, x, y, z;
  SDL_Texture* texture;

  Block();
  Block(float size, float x, float y, float z, SDL_Texture texture);
};

class Air : Block {

};

class Grass : Block {

};

enum BlocksEnum {
  Air = 0,
  Grass
};

void addBlock(float x, float y, float z, std::vector<Renderer::GameObject> &gameObjects);
