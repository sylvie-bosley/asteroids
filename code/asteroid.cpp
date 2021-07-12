#include "include/asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "include/helpers.h"

namespace ag {

Asteroid::Asteroid()
    : position{generate_valid_asteroid_x(), generate_valid_asteroid_y()},
      sprite{50.0f}, orientation{static_cast<float>(rand() % 360u)} {
  initialize_sprite_graphics();
  initialize_sprite_position();
  sf::Vector2f direction(std::sin(orientation * (PI32 / 180.0f)),
                         std::cos(orientation * (PI32 / 180.0f)));
  sf::Vector2f normal_direction = normalize_vector2f(direction);
  velocity = normal_direction * static_cast<float>(ASTEROID_SPEED);
}

void Asteroid::update_pos(const sf::Time dt) {
  sf::Vector2f pos_f;
  position += (velocity * dt.asSeconds());
  position = screen_wrap(position);
  sprite.setPosition(position);
}

void Asteroid::reset_asteroid() {
  orientation = static_cast<float>(rand() % 360u);
  position = {generate_valid_asteroid_x(), generate_valid_asteroid_y()};
  initialize_sprite_position();
}

float Asteroid::generate_valid_asteroid_x() {
  float new_x;
  do {
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
  } while (new_x > (DISPLAY_SIZE.x * 0.33f) &&
           new_x < (DISPLAY_SIZE.x * 0.67f));
  return new_x;
}

float Asteroid::generate_valid_asteroid_y() {
  float new_y;
  do {
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
  } while (new_y > (DISPLAY_SIZE.y * 0.33f) &&
           new_y < (DISPLAY_SIZE.y * 0.67f));
  return new_y;
}

void Asteroid::initialize_sprite_graphics() {
  sprite.setOrigin(50.0f, 50.0f);
  sprite.setOutlineThickness(1.0f);
  sprite.setFillColor(sf::Color::Black);
  sprite.setOutlineColor(sf::Color::White);
}

void Asteroid::initialize_sprite_position() {
  sprite.setPosition(position);
  sprite.setRotation(-orientation);
}

}
