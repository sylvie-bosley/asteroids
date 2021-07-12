#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <SFML/Graphics.hpp>

namespace ag {

class Asteroid {
 public:
  Asteroid() {};
  Asteroid(sf::Vector2f start_position);
  ~Asteroid() {};

  void update_pos(const sf::Time dt);
  void reset_asteroid();

  sf::CircleShape sprite;
  sf::Vector2f position;

 private:
  const unsigned int ASTEROID_SPEED = 100u;

  void initialize_sprite_position();
  void initialize_sprite_graphics();

  sf::Vector2f velocity;
  float orientation;
};

}

#endif
