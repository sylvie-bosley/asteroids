#include "spaceship.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"
#include "bullet.h"
#include "helpers.h"

namespace ag {

Spaceship::Spaceship(const sf::Vector2f starting_pos, const unsigned int id)
    : m_sprite{3U}, m_radius{10.0F}, m_thrust{0.0F}, m_angular_velocity{0.0F},
      m_gun_cd{0.0F}, m_shooting{false} {
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
  m_sprite.move(starting_pos);

#ifdef DEBUG
  initialize_stats_string();
#endif
}

#ifdef DEBUG
bool Spaceship::load_resources(std::string gun_sfx, std::string game_font) {
  bool loaded = true;
  if (!m_gun_sound_buffer.loadFromFile(gun_sfx) ||
      !m_stats_font.loadFromFile(game_font)) {
    loaded - false;
  }
  m_gun_sound.setBuffer(m_gun_sound_buffer);
  m_ship_stats.setFont(m_stats_font);
  return loaded;
}
#else
bool Spaceship::load_resources(std::string gun_sfx) {
  bool loaded = true;
  if (!m_gun_sound_buffer.loadFromFile(gun_sfx)) {
    loaded - false;
  }
  m_gun_sound.setBuffer(m_gun_sound_buffer);
  return loaded;
}
#endif

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

float Spaceship::get_rotation() const {
  return m_sprite.getRotation();
}

bool Spaceship::is_shooting() const {
  return m_shooting;
}

std::shared_ptr<GameObject> Spaceship::spawn_bullet(unsigned int id) {
  sf::Vector2f gun_position = m_sprite.getTransform().transformPoint(
    m_sprite.getPoint(0) - sf::Vector2f{0.0F, 2.0F});
  m_gun_sound.play();
  return std::make_shared<Bullet>(id, m_sprite.getRotation(), get_velocity(),
                                  gun_position, m_radius);
}

void Spaceship::move_to(sf::Vector2f new_position) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
}

void Spaceship::collide() {
  set_destroyed(true);
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
  if (is_shooting()) {
    m_shooting = false;
    m_gun_cd = GUN_COOLDOWN;
  } else if (m_gun_cd > 0.0F) {
    m_gun_cd -= dt;
  }

#ifdef DEBUG
  update_ship_stats();
#endif
}

void Spaceship::control_ship(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    if (m_gun_cd <= 0.0F) {
      m_shooting = true;
    } else {
      // TODO: Add sound effect for still reloading
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

void Spaceship::reset_ship(sf::Vector2f new_position, float new_rotation,
                           sf::Vector2f new_velocity) {
  m_sprite.move(new_position.x - m_sprite.getPosition().x,
                new_position.y - m_sprite.getPosition().y);
  m_sprite.rotate(-(m_sprite.getRotation() - new_rotation));
  m_gun_cd = 0.0F;
  m_shooting = false;
  set_velocity(new_velocity);
  set_destroyed(false);

#ifdef DEBUG
  update_ship_stats();
#endif
}

#ifdef DEBUG
const sf::Text *Spaceship::get_ship_stats() {
  return &m_ship_stats;
}

void Spaceship::initialize_stats_string() {
  m_ship_stats.setCharacterSize(20U);
  m_ship_stats.setFillColor(sf::Color::White);
  m_ship_stats.move(5.0F, 5.0F);
  update_ship_stats();
}

void Spaceship::update_ship_stats() {
  std::string stats_str = "Position: (" +
      std::to_string(m_sprite.getPosition().x) + ", " +
      std::to_string(m_sprite.getPosition().y) + ")\n" + "X Velocity: " +
      std::to_string(get_velocity().x) + "\n" + "Y Velocity: " +
      std::to_string(get_velocity().y) + "\n" + "Rotation: " +
      std::to_string(m_sprite.getRotation());
  m_ship_stats.setString(stats_str);
}
#endif

}
