#include "include/spaceship.h"

#include <cmath>

#include <SFML/Audio.hpp>

#include "include/helpers.h"

namespace ag {

Spaceship::Spaceship()
    : position{static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U)},
      velocity{0.0F, 0.0F}, orientation{0.0F}, ship_sfx_buffer1{},
      ship_sfx_buffer2{}, ship_gun_sound{}, ship_engines_sound{} {}

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
      rotate_ship(1.0F, dt);
      break;
    case Right:
      rotate_ship(-1.0F, dt);
      break;
    default:
      break;
  }
}

void Spaceship::update_pos() {
  position += velocity;
  position = screen_wrap(position);
}

void Spaceship::reset_ship() {
  position = static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U);
  velocity.x = 0.0F;
  velocity.y = 0.0F;
  orientation = 0.0F;
}

void Spaceship::main_thruster(const sf::Time dt) {
  float d_accel = ACCELERATION * dt.asSeconds();
  sf::Vector2f direction(std::sin(orientation * (M_PI / 180.0F)),
                         std::cos(orientation * (M_PI / 180.0F)));
  velocity -= (normalize_vector2f(direction) * d_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::retro_trusters(const sf::Time dt) {
  float d_retro_accel = ACCELERATION * dt.asSeconds() / 2.0F;
  sf::Vector2f direction(std::sin(orientation * (M_PI / 180.0F)),
                         std::cos(orientation * (M_PI / 180.0F)));
  velocity += (normalize_vector2f(direction) * d_retro_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::rotate_ship(const float direction, const sf::Time dt) {
  orientation += (direction * ROTATION_SPEED * dt.asSeconds());
  clamp_orientation();
}

void Spaceship::fire_weapon() {
  ship_sfx_buffer1.loadFromFile("data/test/ball.wav");
  ship_gun_sound.setBuffer(ship_sfx_buffer1);
  ship_gun_sound.play();
}

void Spaceship::clamp_orientation() {
  while (orientation >= 360.0F) {
    orientation -= 360.0F;
  }
  while (orientation < 0.0F) {
    orientation += 360.0F;
  }
}

}
