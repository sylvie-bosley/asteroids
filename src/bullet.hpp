#ifndef ASTEROIDS_GAME_CODE_INCLUDE_BULLET_H
#define ASTEROIDS_GAME_CODE_INCLUDE_BULLET_H

#include <SFML/Graphics.hpp>

#include "game_object.hpp"

namespace ag {

class Bullet : public GameObject {
 public:
  Bullet() {};
  explicit Bullet(unsigned int id, GameObject::ObjectType parent_type,
                  float rotation, sf::Vector2f ship_velocity,
                  sf::Vector2f ship_position, float lifetime);
  ~Bullet() {};

  const sf::Drawable *get_sprite() const override;
  sf::FloatRect get_bounds() const override;
  sf::Vector2f get_position() const override;
  float get_radius() const override;
  void collide() override;
  void move_to(sf::Vector2f new_position) override;
  void update(float dt) override;
  GameObject::ObjectType get_parent_type() const;

 private:
  const float BULLET_SPEED = 250.0F;
  const float BULLET_SIZE = 2.0F;

  float m_ttl;
  sf::CircleShape m_sprite;
  GameObject::ObjectType m_parent_type;
};

}

#endif
