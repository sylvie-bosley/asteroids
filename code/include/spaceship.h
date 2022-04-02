#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Spaceship : public GameObject {
 public:
  Spaceship();
  ~Spaceship() {};

  bool load_resources(std::string gun_sfx/*DEBUG*/,
      std::string font/*END DEBUG*/);
  void control_ship();
  void update(const sf::Time &dt);
  void reset_ship();
  const sf::CircleShape &get_sprite();
  const std::vector<sf::Vector2f> get_vertices() const;

  // DEBUG
  const sf::Text &get_ship_stats();
  // END DEBUG

 private:
  const unsigned int ROTATION_SPEED = 180U;
  const float MAX_SPEED = 300.0F;
  const float ACCELERATION = 0.1F;

  void engage_thrusters(const float direction);
  void fire_weapon();
  float clamp_orientation(const float new_orientation) const;
  void initialize_sprite_graphics();
  void initialize_sprite_position();

  sf::CircleShape m_sprite;
  sf::SoundBuffer m_gun_sound_buffer;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Sound m_gun_sound;
  float m_angular_velocity;

  // DEBUG
  void initialize_stats_string();
  void update_ship_stats();
  sf::Font m_stats_font;
  sf::Text m_ship_stats;
  // END DEBUG
};

}

#endif
