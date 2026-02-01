#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#define windowWidth 800
#define windowHeight 600

SDL_Vertex newVertex(int x, int y, float r, float g, float b) {
  SDL_Vertex newVertex{};
  newVertex.position.x = x;
  newVertex.position.y = y;
  newVertex.color.a = 1.0f;
  newVertex.color.r = r;
  newVertex.color.g = g;
  newVertex.color.b = b;

  return newVertex;
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

  SDL_Vertex vertices[3] = {
      newVertex(windowWidth / 2, 0, 0.0f, 1.0f, 0.0f),
      newVertex(0, windowHeight, 1.0f, 0.0f, 0.0f),
      newVertex(windowWidth, windowHeight, 0.0f, 0.0f, 1.0f)};

  int indices[3] = {0, 1, 2};

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderGeometry(renderer, nullptr, vertices, 3, indices, 3);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
