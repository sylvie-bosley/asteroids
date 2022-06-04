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
                    const sf::Vector2f position, const sf::Vector2f velocity);
  ~Asteroid() {};

  const sf::Drawable *get_sprite() const override;
  const sf::FloatRect get_bounds() const override;
  const sf::Vector2f get_position() const override;
  const float get_mass() const override;
  void update(const sf::Time dt) override;
  void deflect(const sf::Vector2f new_velocity) override;
  void collide() override;
  std::shared_ptr<GameObject> spawn_copy(const unsigned int id,
      const sf::Vector2f position) const override;

 private:
  sf::CircleShape m_sprite;
  bool m_deflected;
};

}

#endif
