#include "include/spaceship.h"
#include "include/helpers.h"

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

namespace ag {

Spaceship::Spaceship(const sf::Vector2i position)
    : position{position}, velocity{0.0f, 0.0f}, orientation{0},
      ship_sfx_buffer1{}, ship_sfx_buffer2{}, ship_gun_sound{},
      ship_engines_sound{} {}

Spaceship::~Spaceship() {}

void Spaceship::control_ship(const Action action, const sf::Time dt) {
  switch (action) {
    case FireGun:
      fire_weapon();
      break;
    case Up:
      main_thruster(dt);
      break;
    case Down:
      retro_trusters(dt);
      break;
    case Left:
      rotate_ship(1, dt);
      break;
    case Right:
      rotate_ship(-1, dt);
      break;
    default:
      break;
  }
}

void Spaceship::update_pos(const sf::Vector2i dspl_size) {
  position.x += static_cast<int>(velocity.x);
  position.y += static_cast<int>(velocity.y);
  if (position.x < 0) {
    position.x = dspl_size.x - 1;
  } else if (position.x > dspl_size.x) {
    position.x = 0;
  }

  if (position.y < 0) {
    position.y = dspl_size.y - 1;
  } else if (position.y > dspl_size.y) {
    position.y = 0;
  }
}

void Spaceship::main_thruster(const sf::Time dt) {
  float d_accel = ACCELERATION * dt.asSeconds();
  sf::Vector2f direction(std::sin(orientation * (PI32 / 180.0f)),
                         std::cos(orientation * (PI32 / 180.0f)));
  velocity -= (normalize_vector2f(direction) * d_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity.x = normal_velocity.x * MAX_SPEED * dt.asSeconds();
    velocity.y = normal_velocity.y * MAX_SPEED * dt.asSeconds();
  }
}

void Spaceship::retro_trusters(const sf::Time dt) {
  float d_retro_accel = ACCELERATION * dt.asSeconds() / 2.0f;
  sf::Vector2f direction(std::sin(orientation * (PI32 / 180.0f)),
                         std::cos(orientation * (PI32 / 180.0f)));
  velocity += (normalize_vector2f(direction) * d_retro_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity.x = normal_velocity.x * MAX_SPEED * dt.asSeconds();
    velocity.y = normal_velocity.y * MAX_SPEED * dt.asSeconds();
  }
}

void Spaceship::rotate_ship(const int direction, const sf::Time dt) {
  float d_rotation = direction * ROTATION_SPEED * dt.asSeconds();
  orientation = static_cast<int>(static_cast<float>(orientation) + d_rotation);
  if (orientation <= 0) {
    orientation = 360 + orientation;
  }
  orientation %= 360;
}

void Spaceship::fire_weapon() {
  ship_sfx_buffer1.loadFromFile("data/test/ball.wav");
  ship_gun_sound.setBuffer(ship_sfx_buffer1);
  ship_gun_sound.play();
}

sf::Vector2f Spaceship::normalize_vector2f(const sf::Vector2f vector) {
  sf::Vector2f normal_vector;
  float length = sqrt(vector.x * vector.x + vector.y * vector.y);
  normal_vector.x = vector.x / length;
  normal_vector.y = vector.y / length;
  return normal_vector;
}

float Spaceship::vector2f_length(const sf::Vector2f vector) {
  float length = sqrt(vector.x * vector.x + vector.y * vector.y);
  return length;
}

}
