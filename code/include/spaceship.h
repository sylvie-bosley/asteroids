#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ag {

class Spaceship {
 public:
  Spaceship();
  ~Spaceship() {};

  bool load_resources(std::string gun_sfx, std::string font);
  void control_ship();
  void update(const sf::Time &dt);
  void reset_ship();
  const sf::CircleShape &get_sprite();
  const sf::Text &get_ship_stats();

 private:
  const unsigned int ROTATION_SPEED = 180U;
  const float MAX_SPEED = 300.0F;
  const float ACCELERATION = 25.0F;

  void update_ship_stats();
  void engage_thrusters(const float magnitude);
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
  float m_angular_velocity;
  sf::CircleShape m_sprite;
  sf::Font m_stats_font;
  sf::Text m_ship_stats;
};

}

#endif
