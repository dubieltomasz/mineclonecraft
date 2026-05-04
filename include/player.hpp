#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <math.h>

class Player {
private:
public:
  float x;
  float y;
  float z;
  float mouseX;
  float mouseY;
  uint32_t renderType = 0;
  const float FOV = 45.0f;

  Player(float x, float y, float z);

  void handleEvent(const SDL_Event& event);
  float getFOV();

  void handleInput(const float& dt);
};
