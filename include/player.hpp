#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <math.h>

class Player {
public:
  float x;
  float y;
  float z;
  float camX;
  float camY;

  Player(float x, float y, float z);

  void handleEvent(const SDL_Event& event);

  void handleInput(const float& dt);
};
