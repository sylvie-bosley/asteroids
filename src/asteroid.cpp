#include "asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(const float size, const unsigned int id) : m_sprite{size} {
  set_object_id(id);
  set_object_type(AsteroidType);
  m_sprite.setOrigin(sf::Vector2f{50.0F, 50.0F});
  m_sprite.setPosition(generate_valid_asteroid_position());
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.setRotation(static_cast<float>(rand() % 360U));
  float r_sin = static_cast<float>(std::sin(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  m_velocity = heading * ASTEROID_SPEED;
}

const sf::Drawable *Asteroid::get_sprite() const {
  return &m_sprite;
}

const sf::FloatRect Asteroid::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

void Asteroid::update(const sf::Time dt) {
  m_sprite.move(m_velocity * dt.asSeconds());
  sf::Vector2f wrapped_position = screen_wrap(m_sprite.getPosition());
  if (wrapped_position != m_sprite.getPosition()) {
    m_sprite.setPosition(wrapped_position);
  }
}

void Asteroid::collide() {
  set_destroyed(true);
}

sf::Vector2f Asteroid::generate_valid_asteroid_position() const {
  float new_x, new_y;
  do {
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
  } while (new_x > (DISPLAY_SIZE.x * 0.33F) &&
           new_x < (DISPLAY_SIZE.x * 0.67F));
  do {
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
  } while (new_y > (DISPLAY_SIZE.y * 0.33F) &&
           new_y < (DISPLAY_SIZE.y * 0.67F));
  return sf::Vector2f{new_x, new_y};
}

const sf::Vector2f Asteroid::get_position() const {
  return m_sprite.getPosition();
}

}
