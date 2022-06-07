#include "asteroid.h"

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(float size, unsigned int id, sf::Vector2f position,
                   float rotation)
    : m_sprite{size} {
  set_object_id(id);
  set_object_type(AsteroidType);
  m_sprite.setRotation(rotation);
  float r_sin = static_cast<float>(std::sin(rotation * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(rotation * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(heading * ASTEROID_SPEED);
  set_destroyed(false);
  m_sprite.setOrigin(sf::Vector2f{size, size});
  m_sprite.setPosition(position);
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
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

float Asteroid::get_rotation() const {
  return m_sprite.getRotation();
}

void Asteroid::move_to(sf::Vector2f position) {
  m_sprite.setPosition(position);
}

void Asteroid::collide() {
  set_destroyed(true);
}

void Asteroid::update(float dt) {
  float r_sin = static_cast<float>(std::sin(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(heading * ASTEROID_SPEED);
  m_sprite.move(get_velocity() * dt);
}

std::shared_ptr<GameObject> Asteroid::spawn_child(const GameObject &parent,
                                                  float direction,
                                                  unsigned int id) {
  std::shared_ptr<Asteroid> new_asteroid;
  new_asteroid = std::make_shared<Asteroid>(parent.get_radius() / 2.0F, id,
                                            parent.get_position(),
                                            parent.get_rotation() + direction);
  new_asteroid->update(new_asteroid->get_radius() / ASTEROID_SPEED);
  return new_asteroid;
}

}
