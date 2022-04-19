#include "asteroid.h"

#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "helpers.h"

namespace ag {

Asteroid::Asteroid(const float size, const unsigned int id,
                   const sf::Vector2f position, const sf::Vector2f velocity)
    : m_sprite{size} {
  set_object_id(id);
  set_object_type(AsteroidType);
  set_velocity(velocity);
  set_destroyed(false);
  m_sprite.setOrigin(sf::Vector2f{50.0F, 50.0F});
  m_sprite.setPosition(position);
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
  return m_sprite.getRadius();
}

void Asteroid::update(const sf::Time dt) {
  m_sprite.move(get_velocity() * dt.asSeconds());
  if (m_deflected) {
    m_deflected = false;
  }
}

void Asteroid::deflect(const sf::Vector2f new_velocity) {
  if (!m_deflected) {
    m_deflected = true;
    set_velocity(new_velocity);
  }
}

void Asteroid::collide() {
  set_destroyed(true);
}

std::shared_ptr<GameObject> Asteroid::spawn_wrapped_copy(const unsigned int id,
    const sf::Vector2f position) const {
  return std::make_shared<Asteroid>(get_mass(), id, position, get_velocity());
}

}
