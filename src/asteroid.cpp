#include "asteroid.h"

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(const float size, const unsigned int id,
    const std::vector<std::shared_ptr<GameObject>> other_objects)
    : m_sprite{size},
      m_mass{size} {
  set_object_id(id);
  set_object_type(AsteroidType);
  float direction = static_cast<float>(rand() % 360U);
  float r_sin = static_cast<float>(std::sin(direction * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(direction * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(heading * ASTEROID_SPEED);
  set_destroyed(false);
  m_sprite.setOrigin(sf::Vector2f{50.0F, 50.0F});
  m_sprite.setPosition(generate_valid_asteroid_position(other_objects));
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
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

const float Asteroid::get_mass() const {
  return m_mass;
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

void Asteroid::clear_deflect() {
  m_deflected = false;
}

void Asteroid::collide() {
  set_destroyed(true);
}

sf::Vector2f Asteroid::generate_valid_asteroid_position(
    const std::vector<std::shared_ptr<GameObject>> other_objects) const {
  float old_x, old_y, new_x, new_y, distance;
  bool invalid;
  do {
    invalid = false;
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
    if ((new_x < 50.1F || new_x > DISPLAY_SIZE.x - 50.1F) &&
        (new_y < 50.1F || new_y > DISPLAY_SIZE.y - 50.1F)) {
      invalid = true;
    }
    for (auto other_object : other_objects) {
      old_x = other_object->get_position().x;
      old_y = other_object->get_position().y;
      distance = sqrt(pow((old_x - new_x), 2) + pow((old_y - new_y), 2));
      if (other_object->get_object_type() == GameObject::PlayerType &&
          distance < static_cast<float>(DISPLAY_SIZE.y) / 6.0F) {
          invalid = true;
      } else if (other_object->get_object_type() == GameObject::AsteroidType &&
                 distance < 110.0F) {
          invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
}

}
