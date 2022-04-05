#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <SFML/Graphics.hpp>

#include "game_object.h"

namespace ag {

class Asteroid : public GameObject {
 public:
  Asteroid() {};
  explicit Asteroid(const float size, const unsigned int id);
  ~Asteroid() {};

  const sf::CircleShape &get_sprite();
  void update(const sf::Time &dt);
  bool collides(const std::vector<sf::Vector2f> player_vertices);

 private:
  const float ASTEROID_SPEED = 25.0F;

  float generate_valid_asteroid_x();
  float generate_valid_asteroid_y();

  sf::CircleShape m_sprite;
  sf::Vector2f m_velocity;
};

}

#endif
