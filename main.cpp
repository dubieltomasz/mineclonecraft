#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <array>
#include <cstddef>

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

class Surface {
public:
  int numVertices;
  SDL_Vertex vertices[3];
  int indices[3];
  int numIndices;

  Surface() {
    this->numVertices = 3;
    this->vertices[0] = newVertex(windowWidth / 2, 0, 0.0f, 1.0f, 0.0f);
    this->vertices[1] = newVertex(0, windowHeight, 1.0f, 0.0f, 0.0f);
    this->vertices[2] = newVertex(windowWidth, windowHeight, 0.0f, 0.0f, 1.0f);
    this->indices[0] = 0;
    this->indices[1] = 1;
    this->indices[2] = 2;
    this->numIndices = 3;
  }

  std::array<SDL_Vertex, 3> getOffset(int x, int y) {
    std::array<SDL_Vertex, 3> newVertices = {vertices[0], vertices[1],
                                             vertices[2]};

    for (SDL_Vertex &vertex : newVertices) {
      vertex.position.x -= x;
      vertex.position.y += y;
    }

    return newVertices;
  }
};

class Player {
public:
  int x;
  int y;
  int z;

  Player(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

void getPlayerInput(Player *player) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);

  if (key_states[SDL_SCANCODE_W]) {
    player->y += 1;
  }

  if (key_states[SDL_SCANCODE_A]) {
    player->x -= 1;
  }

  if (key_states[SDL_SCANCODE_S]) {
    player->y -= 1;
  }

  if (key_states[SDL_SCANCODE_D]) {
    player->x += 1;
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

  Surface *surface = new Surface();
  Player *player = new Player(0, 0, 0);

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
    SDL_RenderGeometry(
        renderer, nullptr, surface->getOffset(player->x, player->y).data(),
        surface->numVertices, surface->indices, surface->numIndices);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
