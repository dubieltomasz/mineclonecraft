#include "./include/player.hpp"
#include "./include/terrainGeneration.hpp"
#include "./include/chunk.hpp"
#include "./include/renderer.hpp"
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
#include <string>
#include <vector>

#define windowWidth 800
#define windowHeight 600

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  
  bool shouldClose = false;
  Uint64 lastCheck = SDL_GetPerformanceCounter(), fpsTime = lastCheck, t, freq;
  int frames = 0;
  std::string fps;

  SDL_Window* window;
  if(!(window = SDL_CreateWindow("MineCloneCraft", windowWidth, windowHeight, 0))) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  Renderer renderer(window, 70, 10, 100);
  if (!renderer.ok()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
    return 1;
  };

  if (!SDL_SetWindowRelativeMouseMode(window, true)) {
    SDL_Log("Could not enable relative mouse mode: %s", SDL_GetError());
    return 1;
  }

  Player player(0.0f, 0.0f, 10.0f);

  std::array<calc::Vec2, 256> v = terrainGeneration::vectors(67);

  constexpr float scale = 0.01f;
  std::vector<Chunk> chunks;

  for(int i = 0; i < 8; ++i) {
    for(int j = 0; j < 8; ++j) {
      chunks.push_back(Chunk(i, 0, j, v, scale));
    }
  }

  terrainGeneration::Terrain terrain = terrainGeneration::Terrain();
  for(const Chunk& chunk : chunks) {
    terrain.loadChunk(chunk);
  }

  std::vector<Surface> surfaces = {};
  terrainGeneration::surfacesFromChunks(surfaces, chunks);

  SDL_Event event;
  while (!shouldClose) {  
    t = SDL_GetPerformanceCounter();
    freq = SDL_GetPerformanceFrequency();
    ++frames;

    float dt = static_cast<float>(t - lastCheck) / freq;
    lastCheck = t;

    if(t - fpsTime >= freq) {
      fps = std::to_string(frames * freq / (t - fpsTime));
      fpsTime = t;
      frames = 0;
    }

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        shouldClose = true;
      }

      player.handleEvent(event);
    }
    player.handleInput(dt);
    player.updateCamera();

    renderer
      .prepare()
      .terrain(surfaces, player)
      .hud({fps, std::to_string(chunks.size())}, {{0, 0}, {0, 10}})
      .render();
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
