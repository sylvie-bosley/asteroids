#include "asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(const float size, const unsigned int id) : m_sprite{size} {
  set_object_id(id);
  m_sprite.setOrigin(sf::Vector2f{50.0F, 50.0F});
  m_sprite.setPosition(sf::Vector2f{generate_valid_asteroid_x(), generate_valid_asteroid_y()});
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.setOutlineColor(sf::Color::White);
  m_sprite.setRotation(static_cast<float>(rand() % 360U));
  float r_sin = static_cast<float>(std::sin(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  m_velocity = heading * ASTEROID_SPEED;
}

const sf::CircleShape Asteroid::get_sprite() {
  return m_sprite;
}

void Asteroid::update(const sf::Time dt) {
  m_sprite.move(m_velocity * dt.asSeconds());
  sf::Vector2f wrapped_position = screen_wrap(m_sprite.getPosition());
  if (wrapped_position != m_sprite.getPosition()) {
    m_sprite.setPosition(wrapped_position);
  }
}

bool Asteroid::collides(const std::vector<sf::Vector2f> vertices) {
  for (unsigned int i = 0; i < 3; ++i) {
    float distance = sqrt(pow((vertices[i].x - m_sprite.getPosition().x), 2) +
                          pow((vertices[i].y - m_sprite.getPosition().y), 2));
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

}
