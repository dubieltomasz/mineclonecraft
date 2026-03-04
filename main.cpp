#include "calc.hpp"
#include "terrainGeneration.hpp"
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
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

#define windowWidth 800
#define windowHeight 600
#define playerSpeed 1.0f
#define fov 90.0f
#define sensitivity 0.05f

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
  int normal;
  float r, b, g;

  GameObject(float x1, float y1, float z1, float x2, float y2, float z2,
             float x3, float y3, float z3, Color color) {
    this->vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
    this->normal = 0;
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


void getPlayerInput(Player *player, const float& dt) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
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

void addBlock(float x, float y, float z, std::vector<GameObject> &gameObjects) {
  // top
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Color::Cyan()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Color::White()});
  // back
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Color::Magenta()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Color::Cyan()});
  // right
  gameObjects.push_back({x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Color::White()});
  gameObjects.push_back({x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         Color::Yellow()});
  // bot
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Color::Yellow()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Color::White()});
  // front
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Color::Cyan()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 1.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         Color::Magenta()});
  // left
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Color::Yellow()});
  gameObjects.push_back({x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 0.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 0.0f, z * 1.0f + 1.0f,
                         x * 1.0f + 0.0f, y * 1.0f + 1.0f, z * 1.0f + 1.0f,
                         Color::White()});
}

int main(int argc, char *argv[]) {
  SDL_Window *window;
  bool done = false;
  Uint64 lastCheck = SDL_GetPerformanceCounter(), fpsTime = lastCheck;
  int frames = 0;
  float fps = 0;

  SDL_Init(SDL_INIT_VIDEO); // Initialize SDL3
  SDL_Log("Video driver: %s", SDL_GetCurrentVideoDriver());

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

  if (!SDL_SetWindowRelativeMouseMode(window, true)) {
    SDL_Log("Failed to enable relative mouse mode: %s", SDL_GetError());
  }

  Player *player = new Player(0.0f, 0.0f, 10.0f);
  std::vector<GameObject> gameObjects;

  std::array<terrainGeneration::Vec2, 256> v = terrainGeneration::vectors(123);

  constexpr float scale = 0.01f;

  for (int x = 0; x < 128; ++x) {
    for (int z = 0; z < 128; ++z) {
      addBlock(
        static_cast<float>(x),
        std::round(
          terrainGeneration::noise(
            static_cast<float>(x) * scale,
            static_cast<float>(z) * scale,
            v
          ) * 20.0f),
        static_cast<float>(z),
        gameObjects
      );
    }
  }

  while (!done) {
    SDL_Event event;
    
    Uint64 t = SDL_GetPerformanceCounter(), freq = SDL_GetPerformanceFrequency();
    float dt = static_cast<float>(t - lastCheck) / freq;
    lastCheck = t;
    ++frames;

    if(t - fpsTime >= freq) {
      fps = static_cast<float>(frames) * freq / (t - fpsTime);
      fpsTime = t;
      frames = 0;
    }

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

    getPlayerInput(player, dt);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    std::sort(gameObjects.begin(), gameObjects.end(),
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

                return std::sqrt(std::pow(player->x - sumX1, 2) +
                                 std::pow(player->y - sumY1, 2) +
                                 std::pow(player->z - sumZ1, 2)) >
                       std::sqrt(std::pow(player->x - sumX2, 2) +
                                 std::pow(player->y - sumY2, 2) +
                                 std::pow(player->z - sumZ2, 2));
              });

    for (GameObject &gameObject : gameObjects) {
      gameObject.render(player->x, player->y, player->z, player->camX,
                        player->camY, renderer);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(renderer, 10, 10, std::to_string(std::round(fps)).c_str());
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
