#include "spaceship.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"
#include "bullet.h"
#include "helpers.h"

namespace ag {

Spaceship::Spaceship(unsigned int id, sf::Vector2f starting_position)
    : m_starting_position{starting_position}, m_sprite{3U}, m_radius{10.0F},
      m_thrust{0.0F}, m_angular_velocity{0.0F}, m_gun_cd{0.0F},
      m_shooting{false}, m_lives{STARTING_LIVES}, m_score{0U} {
  set_object_id(id);
  set_object_type(PlayerType);
  set_velocity(sf::Vector2f{0.0F, 0.0F});
  set_destroyed(false);
  m_sprite.setPointCount(3);
  m_sprite.setPoint(std::size_t(0U), sf::Vector2f{7.50F, 0.0F});
  m_sprite.setPoint(std::size_t(1U), sf::Vector2f{0.0F, 20.0F});
  m_sprite.setPoint(std::size_t(2U), sf::Vector2f{15.0F, 20.0F});
  m_sprite.setOrigin(sf::Vector2f{7.5F, 10.0F});
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.move(m_starting_position);
}

bool Spaceship::load_resources(std::string gun_sfx) {
  if (!m_gun_sound_buffer.loadFromFile(gun_sfx)) {
    return false;
  }
  m_gun_sound.setBuffer(m_gun_sound_buffer);
  return true;
}

const sf::Drawable *Spaceship::get_sprite() const {
  return &m_sprite;
}

sf::FloatRect Spaceship::get_bounds() const {
  return m_sprite.getGlobalBounds();
}

sf::Vector2f Spaceship::get_position() const {
  return m_sprite.getPosition();
}

std::vector<sf::Vector2f> Spaceship::get_vertices() const {
  std::vector<sf::Vector2f> vertices;
  for (unsigned int i = 0U; i < m_sprite.getPointCount(); i++) {
      vertices.push_back(
          m_sprite.getTransform().transformPoint(m_sprite.getPoint(i)));
  }
  return vertices;
}

float Spaceship::get_radius() const {
  return m_radius;
}

bool Spaceship::is_shooting() const {
  return m_shooting;
}

void Spaceship::collide() {
  m_lives--;
  if (m_lives <= 0U) {
    set_destroyed(true);
  } else {
    reset_ship();
  }
}

void Spaceship::move_to(sf::Vector2f new_position) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
}

void Spaceship::update(float dt) {
  double r_sin = std::sin(m_sprite.getRotation() * (M_PI / 180.0F));
  double r_cos = std::cos(m_sprite.getRotation() * (M_PI / 180.0F));
  sf::Vector2f heading{static_cast<float>(r_sin), static_cast<float>(-r_cos)};
  set_velocity(get_velocity() + (heading * m_thrust));
  if (vector2f_length(get_velocity()) > MAX_SPEED) {
    sf::Vector2f normal_velocity = normalize_vector2f(get_velocity());
    set_velocity(normal_velocity * MAX_SPEED);
  }
  m_sprite.move(get_velocity() * dt);
  if (m_angular_velocity != 0.0F) {
    m_sprite.rotate(-(m_angular_velocity * dt));
  }
  if (!m_shooting && m_gun_cd > 0.0F) {
    m_gun_cd -= dt;
  }
}

std::shared_ptr<GameObject> Spaceship::spawn_child(unsigned int id,
                                                   float _direction) {
  m_shooting = false;
  m_gun_cd = GUN_COOLDOWN;
  m_gun_sound.play();
  sf::Vector2f gun_position = m_sprite.getTransform().transformPoint(
    m_sprite.getPoint(0) - sf::Vector2f{0.0F, 3.0F});
  return std::make_shared<Bullet>(id, get_object_type(), m_sprite.getRotation(),
                                  get_velocity(), gun_position, 2.0F);
}

unsigned int Spaceship::get_lives() {
  return m_lives;
}

unsigned int Spaceship::get_score() {
  return m_score;
}

void Spaceship::increment_score(unsigned int increment) {
  unsigned int previous_score = m_score;
  m_score = std::min(m_score + increment, 999999U);
  if (m_score / 50000U > previous_score / 50000U) {
    m_lives++;
  }
}

void Spaceship::control_ship(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    if (m_gun_cd <= 0.0F) {
      m_shooting = true;
    }
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    m_thrust = FORWARD_ACCELERATION;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    m_thrust = REVERSE_ACCELERATION;
  } else {
    m_thrust = 0.0F;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    m_angular_velocity = ROTATION_SPEED;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    m_angular_velocity = -ROTATION_SPEED;
  } else {
    m_angular_velocity = 0.0F;
  }
}

void Spaceship::reset_lives() {
  m_lives = STARTING_LIVES;
}

void Spaceship::reset_score() {
  m_score = 0U;
}

void Spaceship::reset_ship() {
  m_sprite.move(m_starting_position.x - m_sprite.getPosition().x,
                m_starting_position.y - m_sprite.getPosition().y);
  m_sprite.rotate(-(m_sprite.getRotation() - 0.0F));
  m_gun_cd = 0.0F;
  m_shooting = false;
  set_velocity(sf::Vector2f{0.0F, 0.0F});
  set_destroyed(false);
}

}
