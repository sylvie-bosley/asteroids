#include "include/asteroid.h"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "include/helpers.h"

namespace ag {

Asteroid::Asteroid(sf::Vector2i start_position)
    : position{start_position}, sprite(50.0f), orientation{rand() % 360} {
  initialize_sprite_graphics();
  initialize_sprite_position();
  sf::Vector2f direction(std::sin(orientation * (PI32 / 180.0f)),
                         std::cos(orientation * (PI32 / 180.0f)));
  sf::Vector2f normal_direction = normalize_vector2f(direction);
  velocity = normal_direction * ASTEROID_SPEED;
}

void Asteroid::update_pos(const sf::Time dt) {
  sf::Vector2f pos_f;
  pos_f = static_cast<sf::Vector2f>(position) + (velocity * dt.asSeconds());
  position = screen_wrap(static_cast<sf::Vector2i>(pos_f));
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
