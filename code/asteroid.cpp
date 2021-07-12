#include "include/asteroid.h"
#include "include/helpers.h"

#include <SFML/Graphics.hpp>
#include <cmath>

namespace ag {

Asteroid::Asteroid(sf::Vector2i start_position)
    : position{start_position}, sprite(50.0f), orientation{rand() % 360} {
  initialize_sprite_graphics();
  initialize_sprite_position();
  sf::Vector2f direction(std::sin(orientation * (PI32 / 180.0f)),
                         std::cos(orientation * (PI32 / 180.0f)));
  sf::Vector2f normal_direction = normalize_vector2f(direction);
  velocity.x = (normal_direction.x * ASTEROID_SPEED);
  velocity.y = (normal_direction.y * ASTEROID_SPEED);
}

void Asteroid::update_pos(const sf::Time dt) {
  position.x = static_cast<int>(position.x + velocity.x * dt.asSeconds());
  position.y = static_cast<int>(position.y + velocity.y * dt.asSeconds());
  position = screen_wrap(position);
  sprite.setPosition(position.x, position.y);
}

void Asteroid::reset_asteroid() {
  position.x = rand() % DISPLAY_SIZE.x;
  position.y = rand() % DISPLAY_SIZE.y;
  orientation = rand() % 360;
  initialize_sprite_position();
}

void Asteroid::initialize_sprite_position() {
  sprite.setPosition(static_cast<sf::Vector2f>(position));
  sprite.setRotation(-orientation);
}

void Asteroid::initialize_sprite_graphics() {
  sprite.setOrigin(50.0f, 50.0f);
  sprite.setOutlineThickness(1.0f);
  sprite.setFillColor(sf::Color::Black);
  sprite.setOutlineColor(sf::Color::White);
}

}
