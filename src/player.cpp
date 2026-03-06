#include "../include/player.hpp"

#define playerSpeed 1.0f
#define sensitivity 0.05f

Player::Player(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->camX = 0.0f;
  this->camY = 0.0f;
}

void Player::handleEvent(const SDL_Event& event) {
  if (event.type == SDL_EVENT_MOUSE_MOTION) {
    this->camX -= event.motion.xrel * sensitivity;
    this->camY -= event.motion.yrel * sensitivity;
    if (this->camY > 90.0f)
      this->camY = 90.0f;
    if (this->camY < -90.0f)
      this->camY = -90.0f;
  }
}

void Player::handleInput(const float& dt) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
  float degrees = this->camX * M_PI / 180.0f;

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
