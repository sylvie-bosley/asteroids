#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Spaceship : public GameObject {
 public:
  Spaceship();
  explicit Spaceship(const sf::Vector2f starting_pos, const unsigned int id);
  ~Spaceship() {};

#ifdef DEBUG
  bool load_resources(std::string gun_sfx, std::string font);
#else
  bool load_resources(std::string gun_sfx);
#endif

  void control_ship();
  void update(const sf::Time &dt);
  void reset_ship(const sf::Vector2f new_position, const float new_rotation,
                  const sf::Vector2f new_velocity);
  const sf::ConvexShape &get_sprite();
  const std::vector<sf::Vector2f> get_vertices() const;

#ifdef DEBUG
  const sf::Text &get_ship_stats();
#endif

 private:
  const unsigned int ROTATION_SPEED = 180U;
  const float MAX_SPEED = 300.0F;
  const float ACCELERATION = 0.1F;

  void engage_thrusters(const float direction);
  void fire_weapon();

  sf::ConvexShape m_sprite;
  sf::SoundBuffer m_gun_sound_buffer;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Sound m_gun_sound;
  sf::Vector2f m_velocity;
  float m_angular_velocity;

#ifdef DEBUG
  void initialize_stats_string();
  void update_ship_stats();
  sf::Font m_stats_font;
  sf::Text m_ship_stats;
#endif
};

}

#endif
