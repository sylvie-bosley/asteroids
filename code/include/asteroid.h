#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <SFML/Graphics.hpp>

#include "game_object.h"

namespace ag {

class Asteroid : public GameObject {
 public:
  Asteroid() {};
  explicit Asteroid(const float size);
  ~Asteroid() {};

  const sf::CircleShape &get_sprite();
  void update(const sf::Time &dt);
  bool check_for_collision(const std::vector<sf::Vector2f> player_vertices);

private:
  const unsigned int ASTEROID_SPEED = 25U;

  float generate_valid_asteroid_x();
  float generate_valid_asteroid_y();
  void initialize_sprite_graphics();
  void initialize_sprite_position();

  sf::CircleShape m_sprite;
};

}

#endif
