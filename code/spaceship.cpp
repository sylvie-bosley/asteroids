#include "include/spaceship.h"
#include "include/helpers.h"

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

namespace ag {

Spaceship::Spaceship(const sf::Vector2i position)
    : position{position}, velocity{0.0f, 0.0f}, rotation{PI32},
      ship_sfx_buffer1{}, ship_sfx_buffer2{}, ship_gun_sound{},
      ship_engines_sound{} {}

Spaceship::~Spaceship() {}

void Spaceship::control_ship(Action action, sf::Time dt) {
  switch (action) {
    case FireGun:
      fire_weapon();
      break;
    case Up:
      thrust(dt);
      break;
    case Down:
      break;
    case Left:
      break;
    case Right:
      break;
    default:
      break;
  }
}

void Spaceship::update_pos(sf::Vector2i dspl_size) {
  position.x += static_cast<int>(velocity.x);
  position.y += static_cast<int>(velocity.y);
  if (position.x < 0) {
    position.x = dspl_size.x - 1;
  } else if (position.x > dspl_size.x) {
    position.x = 0;
  }

  if (position.y < 0) {
    position.y = dspl_size.y - 1;
  } else if (position.y > dspl_size.y) {
    position.y = 0;
  }
}

void Spaceship::thrust(sf::Time dt) {
  sf::Vector2f direction(std::sin(rotation), std::cos(rotation));
  velocity += (normalize_vector2f(direction) * (ACCELERATION * dt.asSeconds()));
  if (velocity.x > MAX_SPEED * dt.asSeconds()) {
    velocity.x = MAX_SPEED * dt.asSeconds();
  } else if (velocity.x < -MAX_SPEED * dt.asSeconds()) {
    velocity.x = -MAX_SPEED * dt.asSeconds();
  }

  if (velocity.y > MAX_SPEED * dt.asSeconds()) {
    velocity.y = MAX_SPEED * dt.asSeconds();
  } else if (velocity.y < -MAX_SPEED * dt.asSeconds()) {
    velocity.y = -MAX_SPEED * dt.asSeconds();
  }
}

void Spaceship::fire_weapon() {
  ship_sfx_buffer1.loadFromFile("data/test/ball.wav");
  ship_gun_sound.setBuffer(ship_sfx_buffer1);
  ship_gun_sound.play();
}

sf::Vector2f Spaceship::normalize_vector2f(const sf::Vector2f vector) {
  sf::Vector2f normal_vector;
  float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
  normal_vector.x = vector.x / magnitude;
  normal_vector.y = vector.y / magnitude;
  return normal_vector;
}

}
