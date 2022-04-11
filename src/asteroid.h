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
      const std::vector<std::shared_ptr<GameObject>> other_objects);
  ~Asteroid() {};

  const sf::Drawable *get_sprite() const override;
  const sf::FloatRect get_bounds() const override;
  void update(const sf::Time dt) override;
  void collide() override;
  const sf::Vector2f get_position() const override;

 private:
  const float ASTEROID_SPEED = 25.0F;

  sf::Vector2f generate_valid_asteroid_position(
      const std::vector<std::shared_ptr<GameObject>> other_objects) const;
  float generate_valid_asteroid_y();

  sf::CircleShape m_sprite;
  sf::Vector2f m_velocity;
};

}

#endif
