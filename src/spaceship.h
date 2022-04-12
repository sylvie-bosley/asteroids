#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Spaceship : public GameObject {
 public:
  Spaceship() {};
  explicit Spaceship(const sf::Vector2f starting_pos, const unsigned int id);
  ~Spaceship() {};

#ifdef DEBUG
  bool load_resources(const std::string gun_sfx, const std::string game_font);
#else
  bool load_resources(const std::string gun_sfx);
#endif

  const sf::Drawable *get_sprite() const override;
  const sf::FloatRect get_bounds() const override;
  void update(const sf::Time dt) override;
  void collide() override;
  const sf::Vector2f get_position() const override;
  const std::vector<sf::Vector2f> get_vertices() const override;
  void control_ship();
  void reset_ship(const sf::Vector2f new_position, const float new_rotation,
                  const sf::Vector2f new_velocity);

#ifdef DEBUG
  const sf::Text get_ship_stats();
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
