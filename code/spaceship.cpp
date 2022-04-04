#include "include/spaceship.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/game_object.h"
#include "include/helpers.h"

namespace ag {

Spaceship::Spaceship(const sf::Vector2f starting_pos) : m_sprite{3U} {
  m_sprite.setPoint(std::size_t(0U), sf::Vector2f{7.50F, 0.0F});
  m_sprite.setPoint(std::size_t(1U), sf::Vector2f{0.0F, 20.0F});
  m_sprite.setPoint(std::size_t(2U), sf::Vector2f{15.0F, 20.0F});
  m_sprite.setOrigin(7.5F, 10.0F);
  m_sprite.setPosition(starting_pos);
  m_sprite.setOutlineThickness(1.0F);
  m_sprite.setFillColor(sf::Color::Black);
  m_sprite.setOutlineColor(sf::Color::White);
  m_sprite.setRotation(0.0F);
  set_velocity({0.0F, 0.0F});

#ifdef DEBUG
  initialize_stats_string();
#endif
}

#ifdef DEBUG
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

void Spaceship::control_ship() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    fire_weapon();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    engage_thrusters(1.0F);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    engage_thrusters(-0.5F);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    m_angular_velocity = 1.0F * ROTATION_SPEED;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    m_angular_velocity = -1.0F * ROTATION_SPEED;
  }
}

void Spaceship::update(const sf::Time &dt) {
  m_sprite.move(get_velocity() * dt.asSeconds());
  sf::Vector2f wrapped_position = screen_wrap(m_sprite.getPosition());
  if (wrapped_position != m_sprite.getPosition()) {
    m_sprite.setPosition(wrapped_position);
  }
  if (m_angular_velocity != 0.0F) {
    m_sprite.rotate(-(m_angular_velocity * dt.asSeconds()));
    m_angular_velocity = 0.0F;
  }

#ifdef DEBUG
  update_ship_stats();
#endif
}

void Spaceship::reset_ship(const sf::Vector2f new_position,
                           const float new_rotation,
                           const sf::Vector2f new_velocity) {
  m_sprite.setPosition(new_position);
  m_sprite.setRotation(new_rotation);
  set_velocity(new_velocity);
}

const sf::ConvexShape &Spaceship::get_sprite() {
  return m_sprite;
}

const std::vector<sf::Vector2f> Spaceship::get_vertices() const {
  float r_sin = static_cast<float>(std::sin(m_sprite.getRotation() * 
                                            (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(m_sprite.getRotation() * 
                                            (M_PI / 180.0F)));
  sf::Vector2f offset{r_sin, r_cos};
  sf::Vector2f front_tip{m_sprite.getPosition() + offset * 10.0F};

  r_cos = static_cast<float>(std::cos(m_sprite.getRotation() + 120.0F) * 
                                      (M_PI / 180.0F));
  r_sin = static_cast<float>(std::sin(m_sprite.getRotation() + 120.0F) * 
                                      (M_PI / 180.0F));
  offset.x = r_sin;
  offset.y = r_cos;
  sf::Vector2f back_left{m_sprite.getPosition() + offset * 10.0F};

  r_sin = static_cast<float>(std::sin(m_sprite.getRotation() + 240.0F) * 
                                      (M_PI / 180.0F));
  r_cos = static_cast<float>(std::cos(m_sprite.getRotation() + 240.0F) * 
                                      (M_PI / 180.0F));
  offset.x = r_sin;
  offset.y = r_cos;
  sf::Vector2f back_right{m_sprite.getPosition() + offset * 10.0F};

  return std::vector<sf::Vector2f>{front_tip, back_left, back_right};
}

void Spaceship::engage_thrusters(const float direction) {
  float r_sin = static_cast<float>(std::sin(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  float r_cos = static_cast<float>(std::cos(m_sprite.getRotation() *
                                            (M_PI / 180.0F)));
  sf::Vector2f velocity{r_sin, -r_cos};
  set_velocity(get_velocity() + (velocity * ACCELERATION * direction));
  if (vector2f_length(get_velocity()) > MAX_SPEED) {
    sf::Vector2f normal_velocity = normalize_vector2f(get_velocity());
    set_velocity(normal_velocity * MAX_SPEED);
  }
}

void Spaceship::fire_weapon() {
  if (m_gun_sound.getStatus() == sf::Sound::Stopped) {
    m_gun_sound.play();
  }
}

#ifdef DEBUG
const sf::Text &Spaceship::get_ship_stats() {
  return m_ship_stats;
}

void Spaceship::update_ship_stats() {
  std::string stats_str = "X Velocity: " + std::to_string(get_velocity().x) +
      "\n" + "Y Velocity: " + std::to_string(get_velocity().y) + "\n" +
      "Rotation: " + std::to_string(m_sprite.getRotation());
  m_ship_stats.setString(stats_str);
}

void Spaceship::initialize_stats_string() {
  m_ship_stats.setCharacterSize(20U);
  m_ship_stats.setFillColor(sf::Color::White);
  m_ship_stats.setPosition(5.0F, 5.0F);
  m_ship_stats.setString("X Velocity: 0\nY Velocity: 0\nRotation: 0");
}
#endif

}
