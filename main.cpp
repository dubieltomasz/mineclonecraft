#include "./include/player.hpp"
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
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <cmath>
#include <iostream>
#include <vulkan/vulkan_core.h>
#include "./include/terrain.hpp"

#define windowWidth 800
#define windowHeight 600

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  bool shouldClose = false;
  Uint64 lastCheck = SDL_GetPerformanceCounter(), fpsTime = lastCheck, t, freq;
  int frames = 0;

  SDL_Window* window;
  if(!(window = SDL_CreateWindow("MineCloneCraft", windowWidth, windowHeight, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE))) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  {// Scope made just for destructing renderer before window
    Renderer renderer(window);

    if (!SDL_SetWindowRelativeMouseMode(window, true)) {
      SDL_Log("Could not enable relative mouse mode: %s", SDL_GetError());
      return 1;
    }

    Player player(0.0f, 0.0f, 2.0f);
    Terrain terrain = Terrain();
    renderer.createVertexBuffer(terrain.vertices, terrain.vertexBuffer, terrain.vertexBufferMemory);
    renderer.createIndexBuffer(terrain.indices, terrain.indexBuffer, terrain.indexBufferMemory);

    SDL_Event event;
    while (!shouldClose) {  
      t = SDL_GetPerformanceCounter();
      freq = SDL_GetPerformanceFrequency();
      ++frames;

      float dt = static_cast<float>(t - lastCheck) / freq;
      lastCheck = t;

      if(t - fpsTime >= freq) {
        std::cout << std::to_string(frames * freq / (t - fpsTime)) << '\r';
        fpsTime = t;
        frames = 0;
      }

      while (SDL_PollEvent(&event)) {
        if(event.type == SDL_EVENT_QUIT) {
          shouldClose = true;
        }

        if(event.type == SDL_EVENT_WINDOW_RESIZED) {
          renderer.windowResized();
        }

        player.handleEvent(event);
      }
      player.handleInput(dt);

      renderer.drawFrame(&player, terrain);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
