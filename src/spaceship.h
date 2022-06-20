#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Spaceship : public GameObject {
 public:
  Spaceship() {};
  explicit Spaceship(unsigned int id, sf::Vector2f starting_pos);
  ~Spaceship() {};

  bool load_resources(std::string gun_sfx);

  const sf::Drawable *get_sprite() const override;
  sf::FloatRect get_bounds() const override;
  sf::Vector2f get_position() const override;
  std::vector<sf::Vector2f> get_vertices() const override;
  float get_radius() const override;
  bool is_shooting() const override;
  void collide() override;
  void move_to(sf::Vector2f new_position) override;
  void update(float dt) override;
  std::shared_ptr<GameObject> spawn_child(unsigned int id,
                                          float _direction = 0.0F) override;
  unsigned int get_lives();
  unsigned int get_score();
  void increment_score(unsigned int increment);
  void control_ship(float dt);
  void reset_lives();
  void reset_score();
  void reset_ship();

 private:
  const float ROTATION_SPEED = 180.0F;
  const float MAX_SPEED = 300.0F;
  const float FORWARD_ACCELERATION = 0.75F;
  const float REVERSE_ACCELERATION = -0.375F;
  const float GUN_COOLDOWN = 0.5F;
  const unsigned int STARTING_LIVES = 3U;

  sf::Vector2f m_starting_position;
  sf::ConvexShape m_sprite;
  sf::SoundBuffer m_gun_sound_buffer;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Sound m_gun_sound;
  float m_radius;
  float m_thrust;
  float m_angular_velocity;
  float m_gun_cd;
  bool m_shooting;
  unsigned int m_lives;
  unsigned int m_score;
};

}

#endif
