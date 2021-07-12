#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <SFML/Graphics.hpp>

namespace ag {

class Asteroid {
 public:
  Asteroid() {};
  Asteroid(sf::Vector2i start_position);
  ~Asteroid() {};

  void update_pos(const sf::Time dt);
  void reset_asteroid();

  sf::CircleShape sprite;
  sf::Vector2i position;

 private:
  const float ASTEROID_SPEED = 100.0f;

  void initialize_sprite_position();
  void initialize_sprite_graphics();

  sf::Vector2f velocity;
  int orientation;
};

}

#endif
