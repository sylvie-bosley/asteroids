#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Audio.hpp>

#include "helpers.h"

namespace ag {

class Spaceship {
 public:
  Spaceship();
  ~Spaceship() {};

  void control_ship(const Action action, const sf::Time dt);
  void update_pos();
  void reset_ship();

  sf::Vector2i position;
  sf::Vector2f velocity;
  int orientation;

 private:
  sf::SoundBuffer ship_sfx_buffer1;
  sf::SoundBuffer ship_sfx_buffer2;
  sf::Sound ship_gun_sound;
  sf::Sound ship_engines_sound;

  const int ROTATION_SPEED = 180;
  const int ACCELERATION = 25;
  const int MAX_SPEED = 300;

  void main_thruster(const sf::Time dt);
  void retro_trusters(const sf::Time dt);
  void rotate_ship(const int direction, const sf::Time dt);
  void fire_weapon();
};

}

#endif
