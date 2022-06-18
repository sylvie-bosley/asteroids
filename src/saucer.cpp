#include "saucer.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"
#include "bullet.h"
#include "helpers.h"

namespace ag {

Saucer::Saucer(unsigned int id, sf::Vector2f starting_pos, float rotation)
    : m_sprite{4U}, m_gun_cd{0.0F} {
  set_object_id(id);
  set_object_type(SaucerType);
  set_velocity(sf::Vector2f{0.0F, 0.0F});
  set_destroyed(false);
  m_sprite.setPointCount(4);
  m_sprite.setPoint(std::size_t(0U), sf::Vector2f{0.0F, 40.0F});
  m_sprite.setPoint(std::size_t(1U), sf::Vector2f{20.0F, 0.0F});
  m_sprite.setPoint(std::size_t(2U), sf::Vector2f{40.0F, 40.0F});
  m_sprite.setPoint(std::size_t(3U), sf::Vector2f{20.0F, 80.0F});
  m_sprite.setOrigin(sf::Vector2f{20.0F, 40.0F});
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.move(starting_pos);
  m_sprite.rotate(-90.0F + rotation);
  double r_sin = std::sin(m_sprite.getRotation() * (M_PI / 180.0F));
  double r_cos = std::cos(m_sprite.getRotation() * (M_PI / 180.0F));
  sf::Vector2f heading{static_cast<float>(r_sin), static_cast<float>(-r_cos)};
  set_velocity(get_velocity() + (heading * SAUCER_SPEED));
}

bool Saucer::load_resources(std::string gun_sfx) {
  bool loaded = true;
  if (!m_gun_sound_buffer.loadFromFile(gun_sfx)) {
    loaded - false;
  }
  m_gun_sound.setBuffer(m_gun_sound_buffer);
  return loaded;
}

const sf::Drawable *Saucer::get_sprite() const {
  return &m_sprite;
}

sf::FloatRect Saucer::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

sf::Vector2f Saucer::get_position() const {
  return m_sprite.getPosition();
}

std::vector<sf::Vector2f> Saucer::get_vertices() const {
  std::vector<sf::Vector2f> vertices;
  for (unsigned int i = 0U; i < m_sprite.getPointCount(); i++) {
      vertices.push_back(
          m_sprite.getTransform().transformPoint(m_sprite.getPoint(i)));
  }
  return vertices;
}

float Saucer::get_radius() const {
  return 40.0F;
}

bool Saucer::is_shooting() const {
  return m_shooting;
}

void Saucer::collide() {
  set_destroyed(true);
}

void Saucer::move_to(sf::Vector2f new_position) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
}

void Saucer::update(float dt) {
  m_sprite.move(get_velocity() * dt);
  if (m_gun_cd <= 0.0F) {
    m_shooting = true;
  } else {
    m_gun_cd -= dt;
  }
}

std::shared_ptr<GameObject> Saucer::spawn_child(unsigned int id,
                                                float _direction) {
  m_shooting = false;
  m_gun_cd = GUN_COOLDOWN;
  m_gun_sound.play();
  sf::Vector2f gun_position = m_sprite.getTransform().transformPoint(
    m_sprite.getPoint(0) - sf::Vector2f{3.0F, 0.0F});
  return std::make_shared<Bullet>(id, get_object_type(), m_trajectory_a,
                                  m_trajectory_v, gun_position, 4.0F);
}

void Saucer::aim(sf::Vector2f player_position) {
  sf::Vector2f distance_v{player_position.x - m_sprite.getPosition().x,
                          player_position.y - m_sprite.getPosition().y};
  m_trajectory_a = std::atan2(distance_v.y, distance_v.x) * (180.0F / M_PI);
  m_trajectory_a += 90.0F;
  m_trajectory_v = normalize_vector2f(distance_v);
}

}
