#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "helpers.h"

namespace ag {

class Spaceship {
 public:
  Spaceship();
  ~Spaceship() {};

  bool load_resources(std::string gun_sfx, std::string font);
  void control_ship(const Action action, const sf::Time &dt);
  void update();
  void reset_ship();
  const sf::CircleShape &get_sprite();
  const sf::Text &get_ship_stats();

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
  void initialize_stats_string();

  sf::SoundBuffer m_gun_sound_buffer;
  sf::Sound m_gun_sound;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Vector2f m_position;
  sf::Vector2f m_velocity;
  float m_orientation;
  sf::CircleShape m_sprite;
  sf::Font m_stats_font;
  sf::Text m_ship_stats;
};

}

#endif
