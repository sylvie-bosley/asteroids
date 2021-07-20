#include "include/asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "include/helpers.h"

namespace ag {

Asteroid::Asteroid(const float size)
    : m_position{generate_valid_asteroid_x(), generate_valid_asteroid_y()},
      m_sprite{size}, m_orientation{static_cast<float>(rand() % 360U)} {
  initialize_sprite_graphics();
  initialize_sprite_position();
  sf::Vector2f direction(std::sin(m_orientation * (M_PI / 180.0F)),
                         std::cos(m_orientation * (M_PI / 180.0F)));
  sf::Vector2f normal_direction = normalize_vector2f(direction);
  m_velocity = normal_direction * static_cast<float>(ASTEROID_SPEED);
}

const sf::CircleShape &Asteroid::get_sprite() {
  return m_sprite;
}

void Asteroid::update(const sf::Time &dt) {
  m_position += (m_velocity * dt.asSeconds());
  m_position = screen_wrap(m_position);
  m_sprite.setPosition(m_position);
}

float Asteroid::generate_valid_asteroid_x() {
  float new_x;
  do {
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
  } while (new_x > (DISPLAY_SIZE.x * 0.33F) &&
           new_x < (DISPLAY_SIZE.x * 0.67F));
  return new_x;
}

float Asteroid::generate_valid_asteroid_y() {
  float new_y;
  do {
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
  } while (new_y > (DISPLAY_SIZE.y * 0.33F) &&
           new_y < (DISPLAY_SIZE.y * 0.67F));
  return new_y;
}

void Asteroid::initialize_sprite_graphics() {
  m_sprite.setOrigin(50.0F, 50.0F);
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.setOutlineColor(sf::Color::White);
}

void Asteroid::initialize_sprite_position() {
  m_sprite.setPosition(m_position);
  m_sprite.setRotation(-m_orientation);
}

}
