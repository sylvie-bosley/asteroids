#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include "helpers.h"

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

namespace ag {

class Spaceship {
 public:
  Spaceship();
  Spaceship(const sf::Vector2i position);
  ~Spaceship();

  void control_ship(Action action, sf::Time dt);
  void update_pos(sf::Vector2i dspl_size);

  sf::Vector2i position;

 private:
  sf::SoundBuffer ship_sfx_buffer1;
  sf::SoundBuffer ship_sfx_buffer2;
  sf::Sound ship_gun_sound;
  sf::Sound ship_engines_sound;

  const float PI32 = 3.14159265359f;
  const float ROTATION_SPEED = PI32;
  const int ACCELERATION = 150;
  const int MAX_SPEED = 500;

  void main_thruster(sf::Time dt);
  void retro_trusters(sf::Time dt);
  void apply_speed_limit(sf::Time dt);
  void fire_weapon();
  sf::Vector2f normalize_vector2f(const sf::Vector2f vector);

  sf::Vector2f velocity;
  float rotation;
};

}

#endif
