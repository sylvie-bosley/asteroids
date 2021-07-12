#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "helpers.h"

namespace ag {

class Spaceship {
 public:
  Spaceship() {};
  Spaceship(const sf::Font &font);
  ~Spaceship() {};

  void control_ship(const Action action, const sf::Time &dt);
  void update();
  void reset_ship();
  sf::CircleShape get_sprite();
  sf::Text get_ship_stats();

 private:
  const unsigned int ROTATION_SPEED = 180U;
  const unsigned int ACCELERATION = 25U;
  const unsigned int MAX_SPEED = 300U;

  void update_ship_stats();
  void main_thruster(const sf::Time &dt);
  void retro_trusters(const sf::Time &dt);
  void rotate_ship(const float direction, const sf::Time &dt);
  void fire_weapon();
  float clamp_orientation(const float new_orientation);
  void initialize_sprite_graphics();
  void initialize_sprite_position();
  void initialize_stats_string(const sf::Font &font);

  sf::SoundBuffer ship_sfx_buffer1;
  sf::SoundBuffer ship_sfx_buffer2;
  sf::Sound ship_gun_sound;
  sf::Sound ship_engines_sound;
  sf::Vector2f position;
  sf::Vector2f velocity;
  float orientation;
  sf::CircleShape sprite;
  sf::Text ship_stats;
};

}

#endif
