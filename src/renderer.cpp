#include "../include/renderer.hpp"
#include "../include/calc.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define fov 90.0f
#define texturesInTexture 4

namespace id {
std::vector<int> indices = {};
std::vector<SDL_Vertex> triangles = {};
}

Renderer::Renderer(SDL_Window* window, int r, int g, int b) {
  this->renderer = SDL_CreateRenderer(window, NULL);
  this->r = r;
  this->g = g;
  this->b = b;
  SDL_Surface* surface = SDL_LoadPNG("../textures/tri.png");
  this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
  SDL_DestroySurface(surface);
}

Renderer::~Renderer() {
  SDL_DestroyTexture(this->texture);
  SDL_DestroyRenderer(this->renderer);
}

bool Renderer::ok() {
  return this->renderer != nullptr;
}

std::vector<SDL_Vertex> Renderer::makeTrianglesWithIndices(
  const std::vector<Surfacea>& surfaces,
  std::vector<int>& indices,
  const Player& player
) {
  std::vector<SDL_Vertex> result = {};
  float f = 1.0f / std::tan((fov * 0.5f) * M_PI / 180.0f), ww = windowWidth * 0.5f, wh = windowHeight * 0.5f;
  indices.clear();
  indices.reserve(8 * 16 * 16);
  int index = 0;

  for (const Surfacea& surface : surfaces) {
    calc::Vec4 vertexGlobal[4];

    bool ok = true;
    switch(surface.texCorAndNormal & 0b111) {
      case 0b000:
        if(player.y > surface.y) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};
       
        break;
      case 0b001:
        if(player.x > surface.x) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};

        break;
      case 0b010:
        if(player.z > surface.z) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        break;
      case 0b100:
        if(player.y < surface.y) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};
       
        break;
      case 0b101:
        if(player.x < surface.x) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};

        break;
      case 0b110:
        if(player.z < surface.z) {
          ok = false;
        }
        vertexGlobal[0] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};
        break;
    }

    if(!ok) {
      continue;
    }

    calc::Vec4 vertexCamera[4] = {
      player.camera * vertexGlobal[0],
      player.camera * vertexGlobal[1],
      player.camera * vertexGlobal[2],
      player.camera * vertexGlobal[3],
    };

    if (vertexCamera[0].z >= 0.0f || vertexCamera[1].z >= 0.0f || vertexCamera[2].z >= 0.0f || vertexCamera[3].z >= 0.0f) {
      continue;
    }

    int inx = surface.texCorAndNormal >> (32-8);
    float v1 = static_cast<float>(inx) / texturesInTexture;
    float v2 = v1 + 1.0f / texturesInTexture;
    float u1 = inx % texturesInTexture;
    float u2 = u1 + 1.0f / texturesInTexture;
    //float tex1[4] = {v1, v1, v2, v2};
    //float tex2[4] = {u1, u2, u1, u2};
    float tex1[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float tex2[4] = {0.0f, 1.0f, 0.0f, 1.0f};

    for(size_t j = 0; j < 4; ++j) {
      float dx = (vertexCamera[j].x) * f / -vertexCamera[j].z;
      float dy = (vertexCamera[j].y) * f / -vertexCamera[j].z;

      result.push_back({
        dx * ww + ww,
        -dy * wh + wh,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        tex1[j],
        tex2[j],
      });
    }

    indices.push_back(index);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 3);
    index += 4;
  }

  return result;
}

Renderer& Renderer::prepare() {
  SDL_SetRenderDrawColor(this->renderer, this->r, this->g, this->b, 255);
  SDL_RenderClear(this->renderer);

  return *this;
}

Renderer& Renderer::terrain(
  const std::vector<Surfacea>& surfaces,
  const Player& player
) {
  id::triangles = makeTrianglesWithIndices(surfaces, id::indices, player);

  SDL_RenderGeometry(this->renderer, this->texture, id::triangles.data(), id::triangles.size(), id::indices.data(), id::indices.size());

  return *this;
}

Renderer& Renderer::hud(const std::vector<std::string>& strings, const std::vector<std::pair<int, int>>& positions) {
  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  for(size_t i = 0; i < strings.size(); ++i) {
    SDL_RenderDebugText(this->renderer, positions[i].first, positions[i].second, strings[i].c_str());
  }

  return *this;
}

void Renderer::render() {
  SDL_RenderPresent(this->renderer);
}
