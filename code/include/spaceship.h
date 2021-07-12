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

  sf::Vector2f position;
  sf::Vector2f velocity;
  float orientation;

 private:
  sf::SoundBuffer ship_sfx_buffer1;
  sf::SoundBuffer ship_sfx_buffer2;
  sf::Sound ship_gun_sound;
  sf::Sound ship_engines_sound;

  const unsigned int ROTATION_SPEED = 180U;
  const unsigned int ACCELERATION = 25U;
  const unsigned int MAX_SPEED = 300U;

  void main_thruster(const sf::Time dt);
  void retro_trusters(const sf::Time dt);
  void rotate_ship(const float direction, const sf::Time dt);
  void fire_weapon();
  void clamp_orientation();
};

}

#endif
