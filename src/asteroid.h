#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"

namespace ag {

class Asteroid : public GameObject {
 public:
  Asteroid() {};
  explicit Asteroid(const float size, const unsigned int id,
                    const sf::Vector2f position, const float rotation);
  ~Asteroid() {};

  const sf::Drawable *get_sprite() const override;
  const sf::FloatRect get_bounds() const override;
  const sf::Vector2f get_position() const override;
  const float get_radius() const override;
  const float get_rotation() const override;
  void move_to(sf::Vector2f new_position) override;
  void collide() override;
  void update(const float dt) override;
  std::shared_ptr<GameObject> spawn_child(const GameObject &parent,
                                          const float direction,
                                          const unsigned int id) override;

 private:
  const float ASTEROID_SPEED = 50.0F;

  sf::CircleShape m_sprite;
};

}

#endif
