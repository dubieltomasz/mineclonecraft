#include "../include/renderer.hpp"
#include "../include/calc.hpp"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <cmath>
#include <cstdint>
#include <execution>
#include <string>
#include <vector>
#include <algorithm>

#define windowWidth 800
#define windowHeight 600
#define fov 90.0f

namespace id {
std::vector<Tri> indices = {};
std::vector<SDL_Vertex> triangles = {};
}

Renderer::Renderer(SDL_Window* window, int r, int g, int b) {
  this->renderer = SDL_CreateRenderer(window, NULL);
  this->r = r;
  this->g = g;
  this->b = b;
  SDL_Surface* surface = SDL_LoadPNG("../textures/tex.png");
  this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
  SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST);
  SDL_DestroySurface(surface);
}

Renderer::~Renderer() {
  SDL_DestroyTexture(this->texture);
  SDL_DestroyRenderer(this->renderer);
}

bool Renderer::ok() {
  return this->renderer != nullptr;
}

inline float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
  return std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2) + std::pow(z1 - z2, 2);
}

void Renderer::makeTrianglesWithIndices(
  const std::vector<Surface>& surfaces,
  std::vector<SDL_Vertex>& triangles,
  std::vector<Tri>& indices,
  const Player& player
) {
  float f = 1.0f / std::tan((fov * 0.5f) * M_PI / 180.0f), ww = windowWidth * 0.5f, wh = windowHeight * 0.5f;
  triangles.clear();
  indices.clear();
  int index = 0;

  for (const Surface& surface : surfaces) {
    calc::Vec4 vertexGlobal[4];

    switch(surface.texCorAndNormal & SurfaceNormal) {
      case FaceBottom:
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};

        break;
      case FaceLeft:
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};

        break;
      case FaceBack:
        vertexGlobal[0] = {surface.x, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        break;
      case FaceTop:
        vertexGlobal[0] = {surface.x, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};

        break;
      case FaceRight:
        vertexGlobal[0] = {surface.x + 1, surface.y, surface.z, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y + 1, surface.z, 1.0f};
        vertexGlobal[2] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};

        break;
      case FaceFront:
        vertexGlobal[0] = {surface.x, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[1] = {surface.x + 1, surface.y, surface.z + 1, 1.0f};
        vertexGlobal[2] = {surface.x, surface.y + 1, surface.z + 1, 1.0f};
        vertexGlobal[3] = {surface.x + 1, surface.y + 1, surface.z + 1, 1.0f};
        break;
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
    float tileSize = 1.0f / (this->texture->w >> 4), padding = 0.01f;

    float v1 = inx / tileSize, v2 = v1 + tileSize;
    float u1 = std::fmod(inx, (this->texture->w >> 4)) * tileSize, u2 = u1 + tileSize;
    uint8_t ao1 = surface.getAO();
    int ao[4] = {
      ao1 & 3,
      (ao1 >>= 2) & 3,
      (ao1 >>= 2) & 3,
      ao1 >>=2,
    };

    float tex1[4]; ;
    float tex2[4]; ;

    if(this->texture->w == 16) {
      tex1[0] = 0.0f;
      tex1[1] = 0.0f;
      tex1[2] = 1.0f;
      tex1[3] = 1.0f;
      tex2[0] = 0.0f;
      tex2[1] = 1.0f;
      tex2[2] = 0.0f;
      tex2[3] = 1.0f;
    } else {
      tex1[0] = v1;
      tex1[1] = v1;
      tex1[2] = v2;
      tex1[3] = v2;
      tex2[0] = u1;
      tex2[1] = u2;
      tex2[2] = u1;
      tex2[3] = u2;
    }

    for(size_t j = 0; j < 4; ++j) {

      float dx = (vertexCamera[j].x) * f / -vertexCamera[j].z;
      float dy = (vertexCamera[j].y) * f / -vertexCamera[j].z;

      triangles.push_back({
        dx * ww + ww,
        -dy * wh + wh,
        1.0f * ao[j] / 3.0f,
        1.0f * ao[j] / 3.0f,
        1.0f * ao[j] / 3.0f,
        1.0f,
        tex2[j],
        tex1[j],
      });
    }
    
    float dist1 = (vertexCamera[0].z + vertexCamera[1].z + vertexCamera[2].z) / 3.0f;
    float dist2 = (vertexCamera[1].z + vertexCamera[2].z + vertexCamera[3].z) / 3.0f;
    indices.push_back({index, index + 1, index + 2, dist1});
    indices.push_back({index + 1, index + 2, index + 3, dist2});

    index += 4;
  }
}

Renderer& Renderer::prepare() {
  SDL_SetRenderDrawColor(this->renderer, this->r, this->g, this->b, 255);
  SDL_RenderClear(this->renderer);

  return *this;
}

Renderer& Renderer::terrain(
  const std::vector<Surface>& surfaces,
  const Player& player
) {

  makeTrianglesWithIndices(surfaces, id::triangles, id::indices, player);

  std::sort(std::execution::par, id::indices.begin(), id::indices.end(),
    [&](const Tri& a, const Tri& b) -> bool {
      return a.dist < b.dist;
    }
  );

  std::vector<int> ind = {};
  for(const Tri& t : id::indices) {
    ind.push_back(t.ind1);
    ind.push_back(t.ind2);
    ind.push_back(t.ind3);
  }

  SDL_RenderGeometry(this->renderer, this->texture, id::triangles.data(), id::triangles.size(), ind.data(), ind.size());

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
