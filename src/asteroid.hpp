#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.hpp"
#include "display_manager.hpp"

namespace ag {

class Asteroid : public GameObject {
 public:
  static const unsigned int SCORE_VALUE = 100U;

  Asteroid() {};
  explicit Asteroid(unsigned int id, float size,
                    sf::Vector2f position, float rotation);
  ~Asteroid() {};

  const sf::Drawable *get_sprite() const override;
  sf::FloatRect get_bounds() const override;
  sf::Vector2f get_position() const override;
  float get_radius() const override;
  void collide() override;
  void move_to(sf::Vector2f new_position) override;
  void update(float dt) override;
  std::shared_ptr<GameObject> spawn_child(unsigned int id,
                                          float direction) override;

 private:
  const float ASTEROID_SPEED = 25.0F;

  sf::CircleShape m_sprite;
};

}

#endif
