#include "bullet.hpp"

#include <cmath>

#include "game_object.hpp"

namespace ag {

Bullet::Bullet(unsigned int id, GameObject::ObjectType parent_type,
               float rotation, sf::Vector2f ship_velocity,
               sf::Vector2f spawn_position, float lifetime)
    :  m_ttl{lifetime}, m_sprite{BULLET_SIZE}, m_parent_type{parent_type} {
  set_object_id(id);
  set_object_type(BulletType);
  float r_sin = static_cast<float>(std::sin(rotation * (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(rotation * (M_PI / 180.0F)));
  sf::Vector2f heading{r_sin, -r_cos};
  set_velocity(ship_velocity + (heading * BULLET_SPEED));
  set_destroyed(false);
  m_sprite.setOrigin(sf::Vector2f{BULLET_SIZE, 0.0F});
  m_sprite.setFillColor(sf::Color::White);
  m_sprite.move(spawn_position);
  m_sprite.rotate(rotation);
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

void Bullet::collide() {
  set_destroyed(true);
}

void Bullet::move_to(sf::Vector2f new_position) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
}

void Bullet::update(float dt) {
  m_sprite.move(get_velocity() * dt);
  m_ttl -= dt;
  if (m_ttl <= 0.0F) {
    set_destroyed(true);
  }
}

GameObject::ObjectType Bullet::get_parent_type() const {
  return m_parent_type;
}

}
