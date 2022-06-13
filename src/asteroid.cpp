#include "asteroid.h"

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(unsigned int id, float size, sf::Vector2f position,
                   float rotation)
    : m_sprite{size} {
  set_object_id(id);
  set_object_type(AsteroidType);
  float r_sin = static_cast<float>(std::sin(rotation * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(rotation * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(heading * ASTEROID_SPEED);
  not_destroyed();
  m_sprite.setOrigin(sf::Vector2f{size, size});
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.move(position);
  m_sprite.rotate(rotation);
}

const sf::Drawable *Asteroid::get_sprite() const {
  return &m_sprite;
}

sf::FloatRect Asteroid::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

sf::Vector2f Asteroid::get_position() const {
  return m_sprite.getPosition();
}

float Asteroid::get_radius() const {
  return m_sprite.getRadius();
}

void Asteroid::move_to(sf::Vector2f new_position) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
}

void Asteroid::update(float dt) {
  m_sprite.move(get_velocity() * dt);
}

std::shared_ptr<GameObject> Asteroid::spawn_child(unsigned int id,
                                                  float direction) {
  std::shared_ptr<Asteroid> new_asteroid;
  new_asteroid = std::make_shared<Asteroid>(id, m_sprite.getRadius() / 2.0F,
                                            get_position(),
                                            m_sprite.getRotation() + direction);
  new_asteroid->update(new_asteroid->get_radius() / ASTEROID_SPEED);
  return new_asteroid;
}

}
