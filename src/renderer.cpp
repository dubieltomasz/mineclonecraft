#include "../include/renderer.hpp"
#include "../include/calc.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <cstdint>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define fov 90.0f
#define texturesInTexture 4.0f

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

bool Renderer::ok() {
  return this->renderer != nullptr;
}

std::vector<SDL_Vertex> toGlobal(
  const std::vector<std::tuple<float, float, float>>& vertices,
  const std::vector<std::pair<float, float>>& texCoords,
  const std::vector<uint8_t>& normals,
  std::vector<int>& indices,
  const Player& player
) {
  std::vector<SDL_Vertex> result = {};
  float f = 1.0f / std::tan((fov * 0.5f) * M_PI / 180.0f);
  indices.clear();
  int index = 0;

  for (size_t i = 0; i < normals.size(); ++i) {
    size_t vi0 = i << 2, vi1 = vi0 + 1, vi2 = vi1 + 1, vi3 = vi2 + 1;

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

    calc::Vec4 vertexGlobal[4] = {
      {std::get<0>(vertices[vi0]), std::get<1>(vertices[vi0]), std::get<2>(vertices[vi0]), 1.0f},
      {std::get<0>(vertices[vi1]), std::get<1>(vertices[vi1]), std::get<2>(vertices[vi1]), 1.0f},
      {std::get<0>(vertices[vi2]), std::get<1>(vertices[vi2]), std::get<2>(vertices[vi2]), 1.0f},
      {std::get<0>(vertices[vi3]), std::get<1>(vertices[vi3]), std::get<2>(vertices[vi3]), 1.0f},
    };

    calc::Vec4 vertexCamera[4] = {
      player.camera * vertexGlobal[0],
      player.camera * vertexGlobal[1],
      player.camera * vertexGlobal[2],
      player.camera * vertexGlobal[3],
    };
    
    if (vertexCamera[0].z >= 0.0f || vertexCamera[1].z >= 0.0f || vertexCamera[2].z >= 0.0f || vertexCamera[3].z >= 0.0f) {
      continue;
    }

    float v1 = texCoords[i].first / texturesInTexture - 1.0f / texturesInTexture;
    float v2 = texCoords[i].first / texturesInTexture;
    float u1 = texCoords[i].second / texturesInTexture - 1.0f / texturesInTexture;
    float u2 = texCoords[i].first / texturesInTexture;
    //float tex1[4] = {v1, v1, v2, v2};
    //float tex2[4] = {u1, u2, u1, u2};
    float tex1[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float tex2[4] = {0.0f, 1.0f, 0.0f, 1.0f};

    for(size_t j = 0; j < 4; ++j) {
      float dx = (vertexCamera[j].x) * f / -vertexCamera[j].z;
      float dy = (vertexCamera[j].y) * f / -vertexCamera[j].z;

      result.push_back({
        dx * (windowWidth * 0.5f) + windowWidth * 0.5f,
        -dy * (windowHeight * 0.5f) + windowHeight * 0.5f,/*
        std::get<0>(textures[i * 2]),
        std::get<1>(textures[i * 2]),
        std::get<2>(textures[i * 2]),*/
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
  const std::vector<std::tuple<float, float, float>>& vertices,
  const std::vector<std::pair<float, float>>& textures,
  const std::vector<uint8_t>& normals,
  const Player& player
) {
  SDL_Surface* surface = SDL_LoadPNG("../textures/tri.png");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);

  std::vector<int> indices = {};
  std::vector<SDL_Vertex> triangles = toGlobal(vertices, textures, normals, indices, player);

  SDL_RenderGeometry(renderer, texture, triangles.data(), triangles.size(), indices.data(), indices.size());

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
