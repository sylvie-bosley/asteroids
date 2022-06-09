#include "bullet.h"

#include <cmath>

#include "game_object.h"

namespace ag {

Bullet::Bullet(unsigned int id, float rotation, sf::Vector2f ship_velocity,
               sf::Vector2f ship_position)
    : m_sprite{BULLET_SIZE}, m_ttl{BULLET_LIFETIME} {
  set_object_id(id);
  set_object_type(BulletType);
  float r_sin = static_cast<float>(std::sin(rotation * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(rotation * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(ship_velocity + (heading * BULLET_SPEED));
  set_destroyed(false);
  m_sprite.setOrigin(sf::Vector2f{BULLET_SIZE, BULLET_SIZE});
  m_sprite.setPosition(ship_position);
  m_sprite.rotate(rotation);
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::White);
}

const sf::Drawable *Bullet::get_sprite() const {
  return &m_sprite;
}

sf::FloatRect Bullet::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

sf::Vector2f Bullet::get_position() const {
  return m_sprite.getPosition();
}

float Bullet::get_radius() const {
  return m_sprite.getRadius();
}

float Bullet::get_rotation() const {
  return m_sprite.getRotation();
}

void Bullet::move_to(sf::Vector2f position) {
  m_sprite.setPosition(position);
}

void Bullet::collide() {
  set_destroyed(true);
}

void Bullet::update(float dt) {
  m_sprite.move(get_velocity() * dt);
  m_ttl -= dt;
  if (m_ttl <= 0.0F) {
    set_destroyed(true);
  }
}

}
