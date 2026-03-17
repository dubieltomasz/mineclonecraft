#include "../include/player.hpp"

#define playerSpeed 5.0f
#define sensitivity 0.05f

Player::Player(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->rotX = 0.0f;
  this->rotY = 0.0f;
  
  calc::Mat4 rotation = calc::Mat4::MRotationY(this->rotX * M_PI / 180.0f) *  calc::Mat4::MRotationX(this->rotY * M_PI / 180.0f);

  calc::Mat4 translation = calc::Mat4::MIdentity();
  translation(0, 3) = -x;
  translation(1, 3) = -y;
  translation(2, 3) = -z;

  this->camera = rotation.transpose() * translation;
}

void Player::handleEvent(const SDL_Event& event) {
  if (event.type == SDL_EVENT_MOUSE_MOTION) {
    this->rotX -= event.motion.xrel * sensitivity;
    this->rotY -= event.motion.yrel * sensitivity;

    if (this->rotY > 90.0f)
      this->rotY = 90.0f;
    if (this->rotY < -90.0f)
      this->rotY = -90.0f;
  }
}

void Player::handleInput(const float& dt) {
  const bool *key_states = SDL_GetKeyboardState(nullptr);
  float degrees = this->rotX * M_PI / 180.0f;

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

void Player::updateCamera() {
    calc::Mat4 rotation = calc::Mat4::MRotationY(rotX * M_PI / 180.0f) *  calc::Mat4::MRotationX(rotY * M_PI / 180.0f);

    calc::Mat4 translation = calc::Mat4::MIdentity();
    translation(0, 3) = -this->x;
    translation(1, 3) = -this->y;
    translation(2, 3) = -this->z;

    this->camera = rotation.transpose() * translation;
}
