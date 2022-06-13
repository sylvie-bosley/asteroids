#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SAUCER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SAUCER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"

namespace ag {

class Saucer : public GameObject {
 public:
  Saucer() {};
  explicit Saucer(unsigned int id, sf::Vector2f starting_pos, float rotation);
  ~Saucer() {};

  bool load_resources(std::string gun_sfx);
  const sf::Drawable *get_sprite() const override;
  sf::FloatRect get_bounds() const override;
  sf::Vector2f get_position() const override;
  std::vector<sf::Vector2f> get_vertices() const override;
  float get_radius() const override;
  bool is_shooting() const override;
  void move_to(sf::Vector2f new_position) override;
  void aim(sf::Vector2f player_position) override;
  void update(float dt) override;
  std::shared_ptr<GameObject> spawn_child(unsigned int id,
                                          float _direction = 0.0F) override;

 private:
  const float SAUCER_SPEED = 100.0F;
  const float GUN_COOLDOWN = 1.0F;

  sf::ConvexShape m_sprite;
  sf::SoundBuffer m_gun_sound_buffer;
  sf::SoundBuffer m_thruster_sound_buffer;
  sf::Sound m_gun_sound;
  sf::Vector2f m_trajectory_v;
  float m_trajectory_a;
  float m_gun_cd;
  bool m_shooting;
};

}

#endif
