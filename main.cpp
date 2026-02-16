#include "calc.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <cstddef>
#include <tuple>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define playerSpeed 0.1f
#define fov 30.0f

namespace Matrix {};

std::vector<SDL_Vertex>
ProjectToScreen(const std::vector<std::tuple<float, float, float>> &vertices,
                float camX, float camY, float camZ) {
  std::vector<SDL_Vertex> result;

  for (const auto &[x, y, z] : vertices) {
    float dz = z - camZ;

    if (dz >= 0.0f) {
      continue;
    }

    float dx = (x - camX) / -dz;
    float dy = (y - camY) / -dz;

    SDL_Vertex newVertex = {};

    newVertex.position.x = dx * fov + windowWidth * 0.5f;
    newVertex.position.y = -dy * fov + windowHeight * 0.5f;
    newVertex.color.a = 1.0f;
    newVertex.color.r = 1.0f;
    newVertex.color.g = 1.0f;
    newVertex.color.b = 1.0f;

    result.push_back(newVertex);
  }

  return result;
}

class GameObject {
public:
  float x;
  float y;
  float z;
  std::vector<std::tuple<float, float, float>> vertices;
  std::vector<int> indices;

  GameObject(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->vertices = {
        {0.0f, 0.0f, 0.0f}, {5.0f, 8.0f, 0.0f}, {10.0f, 0.0f, 0.0f}};
    this->indices = {0, 1, 2};
  }

  void render(int cameraX, int cameraY, int cameraZ, SDL_Renderer *renderer) {
    auto tmp = ProjectToScreen(this->vertices, cameraX, cameraY, cameraZ);
    SDL_RenderGeometry(renderer, nullptr, tmp.data(), tmp.size(), nullptr,
                       tmp.size());
  }
};

class Player {
public:
  float x;
  float y;
  float z;

  Player(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

Uint64 lastCheck = SDL_GetPerformanceCounter();

void getPlayerInput(Player *player) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
  Uint64 t = SDL_GetPerformanceCounter();
  float dt =
      static_cast<float>(t - lastCheck) * 1000 / SDL_GetPerformanceFrequency();
  lastCheck = t;

  if (key_states[SDL_SCANCODE_W]) {
    player->z -= playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_A]) {
    player->x -= playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_S]) {
    player->z += playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_D]) {
    player->x += playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_SPACE]) {
    player->y += playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_LSHIFT]) {
    player->y -= playerSpeed * dt;
  }
}

int main(int argc, char *argv[]) {
  SDL_Window *window;
  bool done = false;

  SDL_Init(SDL_INIT_VIDEO); // Initialize SDL3

  window = SDL_CreateWindow("MineCloneCraft", windowWidth, windowHeight, 0);

  // Check that the window was successfully created
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                 SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n",
                 SDL_GetError());
    return 1;
  }

  Player *player = new Player(0.0f, 0.0f, 3.0f);
  std::vector<GameObject> gameObjects = {};
  gameObjects.push_back({0.0f, 0.0f, 0.0f});

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    getPlayerInput(player);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (GameObject &gameObject : gameObjects) {
      gameObject.render(player->x, player->y, player->z, renderer);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
