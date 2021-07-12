#include "include/spaceship.h"
#include "include/helpers.h"

#include <SFML/Audio.hpp>
#include <cmath>

namespace ag {

Spaceship::Spaceship()
    : position{DISPLAY_SIZE / 2}, velocity{0.0f, 0.0f}, orientation{0},
      ship_sfx_buffer1{}, ship_sfx_buffer2{}, ship_gun_sound{},
      ship_engines_sound{} {}

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

void Spaceship::update_pos() {
  position.x += static_cast<int>(velocity.x);
  position.y += static_cast<int>(velocity.y);
  position = screen_wrap(position);
}

void Spaceship::reset_ship() {
  position = DISPLAY_SIZE / 2;
  velocity.x = 0;
  velocity.y = 0;
  orientation = 0;
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

}
