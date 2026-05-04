#include "../include/player.hpp"

#include "../include/calc.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

#define playerSpeed 5.0f
#define sensitivity 0.05f

Player::Player(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->mouseX = 0.0f;
  this->mouseY = 0.0f;
}

void Player::handleEvent(const SDL_Event& event) {
  if(event.type == SDL_EVENT_MOUSE_MOTION) {
    this->mouseX -= event.motion.xrel * sensitivity;
    this->mouseY -= event.motion.yrel * sensitivity;

    if (this->mouseY > 90.0f)
      this->mouseY = 90.0f;
    if (this->mouseY < -90.0f)
      this->mouseY = -90.0f;
  }

  if(event.type == SDL_EVENT_KEY_DOWN) {
    if(event.key.key == SDLK_P) {
      if(this->renderType == 0) {
        this->renderType = 1;
      } else {
        this->renderType = 0;
      }
    }
  }
}

void Player::handleInput(const float& dt) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
  float degrees = calc::degrees(this->mouseX);

  if (key_states[SDL_SCANCODE_W]) {
    this->z -= std::cos(degrees) * playerSpeed * dt;
    this->x -= std::sin(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_A]) {
    this->z += std::sin(degrees) * playerSpeed * dt;
    this->x -= std::cos(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_S]) {
    this->z += std::cos(degrees) * playerSpeed * dt;
    this->x += std::sin(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_D]) {
    this->z -= std::sin(degrees) * playerSpeed * dt;
    this->x += std::cos(degrees) * playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_SPACE]) {
    this->y += playerSpeed * dt;
  }

  if (key_states[SDL_SCANCODE_LSHIFT]) {
    this->y -= playerSpeed * dt;
  }
}

float Player::getFOV() {
  return this->FOV;
}
