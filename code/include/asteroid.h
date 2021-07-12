#ifndef ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H
#define ASTEROIDS_GAME_CODE_INCLUDE_ASTEROID_H

#include <SFML/Graphics.hpp>

namespace ag {

class Asteroid {
 public:
  Asteroid() {};
  explicit Asteroid(const float size);
  ~Asteroid() {};

  const sf::CircleShape &get_sprite();
  void update(const sf::Time &dt);

private:
  const unsigned int ASTEROID_SPEED = 100U;

  float generate_valid_asteroid_x();
  float generate_valid_asteroid_y();
  void initialize_sprite_graphics();
  void initialize_sprite_position();

  sf::Vector2f velocity;
  float orientation;
  sf::CircleShape sprite;
  sf::Vector2f position;
};

}

#endif
