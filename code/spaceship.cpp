#include "include/spaceship.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/helpers.h"

namespace ag {

Spaceship::Spaceship()
    : m_position{static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U)},
      m_velocity{0.0F, 0.0F}, m_orientation{0.0F}, m_sprite{10.0F, 3U} {
  initialize_sprite_graphics();
  initialize_sprite_position();
  initialize_stats_string();
}

bool Spaceship::load_resources(std::string gun_sfx, std::string font) {
  bool loaded = true;
  if (!m_gun_sound_buffer.loadFromFile(gun_sfx) ||
      !m_stats_font.loadFromFile(font)) {
    loaded - false;
  }
  m_gun_sound.setBuffer(m_gun_sound_buffer);
  m_ship_stats.setFont(m_stats_font);
  return loaded;
}

void Spaceship::control_ship(const Action action, const sf::Time &dt) {
  switch (action) {
    case Space:
      fire_weapon();
      break;
    case Up:
      main_thruster(dt);
      break;
    case Down:
      retro_trusters(dt);
      break;
    case Left:
      rotate_ship(1.0F, dt);
      break;
    case Right:
      rotate_ship(-1.0F, dt);
      break;
    default:
      break;
  }
}

void Spaceship::update() {
  m_position += m_velocity;
  m_position = screen_wrap(m_position);
  m_sprite.setPosition(m_position);
  update_ship_stats();
}

void Spaceship::reset_ship() {
  m_position = static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U);
  m_velocity.x = 0.0F;
  m_velocity.y = 0.0F;
  m_orientation = 0.0F;
  initialize_sprite_position();
}

const sf::CircleShape &Spaceship::get_sprite() {
  return m_sprite;
}

const sf::Text &Spaceship::get_ship_stats() {
  return m_ship_stats;
}

void Spaceship::update_ship_stats() {
  std::string stats_str = "X Velocity: " + std::to_string(m_velocity.x) + "\n" +
                          "Y Velocity: " + std::to_string(m_velocity.y) + "\n" +
                          "Rotation: " + std::to_string(m_orientation);
  m_ship_stats.setString(stats_str);
}

void Spaceship::main_thruster(const sf::Time &dt) {
  float d_accel = ACCELERATION * dt.asSeconds();
  sf::Vector2f direction(std::sin(m_orientation * (M_PI / 180.0F)),
                         std::cos(m_orientation * (M_PI / 180.0F)));
  m_velocity -= (normalize_vector2f(direction) * d_accel);
  if (vector2f_length(m_velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(m_velocity);
    m_velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::retro_trusters(const sf::Time &dt) {
  float d_retro_accel = ACCELERATION * dt.asSeconds() / 2.0F;
  sf::Vector2f direction(std::sin(m_orientation * (M_PI / 180.0F)),
                         std::cos(m_orientation * (M_PI / 180.0F)));
  m_velocity += (normalize_vector2f(direction) * d_retro_accel);
  if (vector2f_length(m_velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(m_velocity);
    m_velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::rotate_ship(const float direction, const sf::Time &dt) {
  float angular_velocity = direction * ROTATION_SPEED * dt.asSeconds();
  m_orientation = clamp_orientation(m_orientation + angular_velocity);
  m_sprite.rotate(-angular_velocity);
}

void Spaceship::fire_weapon() {
  if (m_gun_sound.getStatus() == sf::Sound::Stopped) {
    m_gun_sound.play();
  }
}

float Spaceship::clamp_orientation(const float raw_orientation) {
  float clamped_orientation = raw_orientation;
  while (clamped_orientation >= 360.0F) {
    clamped_orientation -= 360.0F;
  }
  while (clamped_orientation < 0.0F) {
    clamped_orientation += 360.0F;
  }
  return clamped_orientation;
}

void Spaceship::initialize_sprite_graphics() {
  m_sprite.setOrigin(10.0F, 10.0F);
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.setOutlineColor(sf::Color::White);
}

void Spaceship::initialize_sprite_position() {
  m_sprite.setPosition(m_position);
  m_sprite.setRotation(-m_orientation);
}

void Spaceship::initialize_stats_string() {
  m_ship_stats.setCharacterSize(20U);
  m_ship_stats.setFillColor(sf::Color::White);
  m_ship_stats.setPosition(5.0F, 5.0F);
  m_ship_stats.setString("X Velocity: 0\nY Velocity: 0\nRotation: 0");
}

}
