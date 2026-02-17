#include "calc.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <cmath>
#include <cstddef>
#include <tuple>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define playerSpeed 1.0f
#define fov 90.0f
#define sensitivity 0.05f

namespace Matrix {};

class Color {
public:
  float r, g, b;

  Color(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }

  static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }

  static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }

  static Color White() { return Color(1.0f, 1.0f, 1.0f); }
};

std::vector<SDL_Vertex>
ProjectToScreen(const std::vector<std::tuple<float, float, float>> &vertices,
                float camX, float camY, float camZ, float camRotX,
                float camRotY, float r, float b, float g) {
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

class GameObject {
public:
  std::vector<std::tuple<float, float, float>> vertices;
  float r, b, g;

  GameObject(float x1, float y1, float z1, float x2, float y2, float z2,
             float x3, float y3, float z3, Color color) {
    this->vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
    this->r = color.r;
    this->b = color.b;
    this->g = color.g;
  }

  void render(float cameraX, float cameraY, float cameraZ, float camRotX,
              float camRotY, SDL_Renderer *renderer) {
    auto tmp = ProjectToScreen(this->vertices, cameraX, cameraY, cameraZ,
                               camRotX, camRotY, this->r, this->b, this->g);
    SDL_RenderGeometry(renderer, nullptr, tmp.data(), tmp.size(), nullptr,
                       tmp.size());
  }
};

class Player {
public:
  float x;
  float y;
  float z;
  float camX;
  float camY;

  Player(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->camX = 0.0f;
    this->camY = 0.0f;
  }
};

Uint64 lastCheck = SDL_GetPerformanceCounter();

void getPlayerInput(Player *player) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
  Uint64 t = SDL_GetPerformanceCounter();
  float dt = static_cast<float>(t - lastCheck) / SDL_GetPerformanceFrequency();
  lastCheck = t;
  float degrees = player->camX * M_PI / 180.0f;

  if (key_states[SDL_SCANCODE_W]) {
    player->z -= std::cos(degrees) * playerSpeed * dt;
    player->x -= std::sin(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_A]) {
    player->z += std::sin(degrees) * playerSpeed * dt;
    player->x -= std::cos(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_S]) {
    player->z += std::cos(degrees) * playerSpeed * dt;
    player->x += std::sin(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_D]) {
    player->z -= std::sin(degrees) * playerSpeed * dt;
    player->x += std::cos(degrees) * playerSpeed * dt;
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

  SDL_CaptureMouse(true);

  Player *player = new Player(0.0f, 0.0f, 10.0f);
  std::vector<GameObject> gameObjects = {};
  // top
  gameObjects.push_back(
      {0.0f, 5.0f, 0.0f, 5.0f, 5.0f, 0.0f, 5.0f, 5.0f, 5.0f, Color::Cyan()});
  gameObjects.push_back(
      {0.0f, 5.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, Color::White()});
  // back
  gameObjects.push_back(
      {0.0f, 5.0f, 0.0f, 5.0f, 5.0f, 0.0f, 5.0f, 0.0f, 0.0f, Color::Magenta()});
  gameObjects.push_back(
      {0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, Color::Cyan()});
  // right
  gameObjects.push_back(
      {5.0f, 5.0f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 0.0f, 0.0f, Color::White()});
  gameObjects.push_back(
      {5.0f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 0.0f, 0.0f, Color::Yellow()});
  // bot
  gameObjects.push_back(
      {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 0.0f, 5.0f, Color::Yellow()});
  gameObjects.push_back(
      {0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 5.0f, 0.0f, 5.0f, Color::White()});
  // front
  gameObjects.push_back(
      {0.0f, 5.0f, 5.0f, 0.0f, 0.0f, 5.0f, 5.0f, 0.0f, 5.0f, Color::Cyan()});
  gameObjects.push_back(
      {0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 0.0f, 5.0f, Color::Magenta()});
  // left
  gameObjects.push_back(
      {0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 5.0f, 5.0f, Color::Yellow()});
  gameObjects.push_back(
      {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 5.0f, 5.0f, Color::White()});

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }

      if (event.type == SDL_EVENT_MOUSE_MOTION) {
        player->camX -= event.motion.xrel * sensitivity;
        player->camY -= event.motion.yrel * sensitivity;
        if (player->camY > 90.0f)
          player->camY = 90.0f;
        if (player->camY < -90.0f)
          player->camY = -90.0f;
      }
    }

    getPlayerInput(player);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (GameObject &gameObject : gameObjects) {
      gameObject.render(player->x, player->y, player->z, player->camX,
                        player->camY, renderer);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
