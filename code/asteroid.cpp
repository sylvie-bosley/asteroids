#include "include/asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "include/game_object.h"
#include "include/helpers.h"

namespace ag {

Asteroid::Asteroid(const float size) : m_sprite{size} {
  set_position({generate_valid_asteroid_x(), generate_valid_asteroid_y()});
  set_orientation(static_cast<float>(rand() % 360U));
  initialize_sprite_graphics();
  initialize_sprite_position();
  float a_sin = static_cast<float>(std::sin(get_orientation() * (M_PI / 180.0F)));
  float a_cos = static_cast<float>(std::cos(get_orientation() * (M_PI / 180.0F)));
  sf::Vector2f orientation_v{a_sin, a_cos};
  set_velocity(orientation_v * static_cast<float>(ASTEROID_SPEED));
}

const sf::CircleShape &Asteroid::get_sprite() {
  return m_sprite;
}

void Asteroid::update(const sf::Time &dt) {
  set_position(screen_wrap(get_position() + (get_velocity() * dt.asSeconds())));
  m_sprite.setPosition(get_position());
}

// TODO: Make this a template that accepts both ship types
bool Asteroid::check_for_collision(const std::vector<sf::Vector2f> player_vertices) {
  for (unsigned int i = 0; i < 3; ++i) {
    float distance = sqrt(pow((player_vertices[i].x - get_position().x), 2) +
                          pow((player_vertices[i].y - get_position().y), 2));
    if (distance < 50.0F) {
      return true;
    }
  }
  return false;
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
  m_sprite.setPosition(get_position());
  m_sprite.setRotation(-get_orientation());
}

}
