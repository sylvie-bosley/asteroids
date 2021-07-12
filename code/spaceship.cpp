#include "include/spaceship.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/helpers.h"

namespace ag {

Spaceship::Spaceship()
    : position{static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U)},
      velocity{0.0F, 0.0F}, orientation{0.0F}, sprite{10.0F, 3U} {
  if (!stats_font.loadFromFile("data/test/sansation.ttf") ||
      !gun_sound_buffer.loadFromFile("data/test/ball.wav")) {
    // TODO: Not this
    exit(1);
  }
  gun_sound.setBuffer(gun_sound_buffer);
  initialize_sprite_graphics();
  initialize_sprite_position();
  initialize_stats_string();
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
  position += velocity;
  position = screen_wrap(position);
  sprite.setPosition(position);
  update_ship_stats();
}

void Spaceship::reset_ship() {
  position = static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U);
  velocity.x = 0.0F;
  velocity.y = 0.0F;
  orientation = 0.0F;
  initialize_sprite_position();
}

sf::CircleShape Spaceship::get_sprite() {
  return sprite;
}

sf::Text Spaceship::get_ship_stats() {
  return ship_stats;
}

void Spaceship::update_ship_stats() {
  std::string stats_str = "X Velocity: " + std::to_string(velocity.x) + "\n" +
                          "Y Velocity: " + std::to_string(velocity.y) + "\n" +
                          "Rotation: " + std::to_string(orientation);
  ship_stats.setString(stats_str);
}

void Spaceship::main_thruster(const sf::Time &dt) {
  float d_accel = ACCELERATION * dt.asSeconds();
  sf::Vector2f direction(std::sin(orientation * (M_PI / 180.0F)),
                         std::cos(orientation * (M_PI / 180.0F)));
  velocity -= (normalize_vector2f(direction) * d_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::retro_trusters(const sf::Time &dt) {
  float d_retro_accel = ACCELERATION * dt.asSeconds() / 2.0F;
  sf::Vector2f direction(std::sin(orientation * (M_PI / 180.0F)),
                         std::cos(orientation * (M_PI / 180.0F)));
  velocity += (normalize_vector2f(direction) * d_retro_accel);
  if (vector2f_length(velocity) > (MAX_SPEED * dt.asSeconds())) {
    sf::Vector2f normal_velocity = normalize_vector2f(velocity);
    velocity = normal_velocity * (MAX_SPEED * dt.asSeconds());
  }
}

void Spaceship::rotate_ship(const float direction, const sf::Time &dt) {
  float angular_velocity = direction * ROTATION_SPEED * dt.asSeconds();
  orientation = clamp_orientation(orientation + angular_velocity);
  sprite.rotate(-angular_velocity);
}

void Spaceship::fire_weapon() {
  if (gun_sound.getStatus() == sf::Sound::Stopped) {
    gun_sound.play();
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
  sprite.setOrigin(10.0F, 10.0F);
  sprite.setOutlineThickness(1.0F);
  sprite.setFillColor(sf::Color::Black);
  sprite.setOutlineColor(sf::Color::White);
}

void Spaceship::initialize_sprite_position() {
  sprite.setPosition(position);
  sprite.setRotation(-orientation);
}

void Spaceship::initialize_stats_string() {
  ship_stats.setFont(stats_font);
  ship_stats.setCharacterSize(20U);
  ship_stats.setFillColor(sf::Color::White);
  ship_stats.setPosition(5.0F, 5.0F);
  ship_stats.setString("X Velocity: 0\nY Velocity: 0\nRotation: 0");
}

}
