#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Spaceship : public GameObject {
 public:
  Spaceship() {};
  explicit Spaceship(sf::Vector2f starting_pos, unsigned int id);
  ~Spaceship() {};

#ifdef DEBUG
  bool load_resources(std::string gun_sfx, std::string game_font);
#else
  bool load_resources(std::string gun_sfx);
#endif

  const sf::Drawable *get_sprite() const override;
  sf::FloatRect get_bounds() const override;
  sf::Vector2f get_position() const override;
  std::vector<sf::Vector2f> get_vertices() const override;
  float get_radius() const override;
  float get_rotation() const override;
  bool is_shooting() const override;
  std::shared_ptr<GameObject> spawn_bullet(unsigned int id) override;
  void move_to(sf::Vector2f new_position) override;
  void collide() override;
  void update(float dt) override;
  void control_ship(float dt);
  void reset_ship(sf::Vector2f new_position, float new_rotation,
                  sf::Vector2f new_velocity);

#ifdef DEBUG
  const sf::Text *get_ship_stats();
#endif

 protected:
  void set_radius(float radius) override;

 private:
  const unsigned int ROTATION_SPEED = 180U;
  const float MAX_SPEED = 300.0F;
  const float ACCELERATION = 0.75F;
  const float GUN_COOLDOWN = 0.5F;

  void engage_thrusters(float direction);
  void fire_weapon();

  sf::ConvexShape m_sprite;
  sf::SoundBuffer m_gun_sound_buffer;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Sound m_gun_sound;
  float m_angular_velocity;
  float m_radius;
  bool m_shooting;
  float m_gun_cd;

#ifdef DEBUG
  void initialize_stats_string();
  void update_ship_stats();
  sf::Font m_stats_font;
  sf::Text m_ship_stats;
#endif
};

}

#endif
