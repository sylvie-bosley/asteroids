#include "asteroid.h"

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(const float size, const unsigned int id,
    const std::vector<std::shared_ptr<GameObject>> other_objects)
    : m_sprite{size},
      m_deflected{false} {
  set_object_id(id);
  set_object_type(AsteroidType);
  m_sprite.setOrigin(sf::Vector2f{50.0F, 50.0F});
  m_sprite.setPosition(generate_valid_asteroid_position(other_objects));
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  float direction = static_cast<float>(rand() % 360U);
  float r_sin = static_cast<float>(std::sin(direction * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(direction * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(heading * ASTEROID_SPEED);
}

const sf::Drawable *Asteroid::get_sprite() const {
  return &m_sprite;
}

const sf::FloatRect Asteroid::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

const sf::Vector2f Asteroid::get_position() const {
  return m_sprite.getPosition();
}

void Asteroid::update(const sf::Time dt) {
  m_sprite.move(get_velocity() * dt.asSeconds());
  sf::Vector2f wrapped_position = screen_wrap(m_sprite.getPosition());
  if (wrapped_position != m_sprite.getPosition()) {
    m_sprite.setPosition(wrapped_position);
  }
  if (m_deflected) {
    m_deflected = false;
  }
}

void Asteroid::deflect(const sf::Vector2f other_velocity) {
  if (!m_deflected) {
    m_deflected = true;
    set_velocity(other_velocity);
  }
}

void Asteroid::collide() {
  set_destroyed(true);
}

sf::Vector2f Asteroid::generate_valid_asteroid_position(
    const std::vector<std::shared_ptr<GameObject>> other_objects) const {
  float new_x, new_y;
  bool invalid;
  do {
    invalid = false;
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
    for (unsigned int i = 0U; i < other_objects.size(); i++) {
      if (other_objects.at(i)->get_object_type() == GameObject::PlayerType &&
          abs(new_x - other_objects.at(i)->get_position().x) <
            (DISPLAY_SIZE.x / 6.0F)) {
        invalid = true;
      }
      if (other_objects.at(i)->get_object_type() == GameObject::AsteroidType &&
          abs(new_x - other_objects.at(i)->get_position().x) < 60.0F) {
        invalid = true;
      }
    }
  } while (invalid);
  do {
    invalid = false;
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
    for (unsigned int i = 0U; i < other_objects.size(); i++) {
      if (other_objects.at(i)->get_object_type() == GameObject::PlayerType &&
          abs(new_y - other_objects.at(i)->get_position().y) <
            (DISPLAY_SIZE.y / 6.0F)) {
        invalid = true;
      }
      if (other_objects.at(i)->get_object_type() == GameObject::AsteroidType &&
          abs(new_y - other_objects.at(i)->get_position().y) < 60.0F) {
        invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
}

}
