#ifndef ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H
#define ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H

#include <SFML/System.hpp>

namespace ag {

constexpr int MAX_ASTEROIDS = 40;
constexpr float PI32 = 3.14159265359f;
const sf::Vector2i DISPLAY_SIZE{1280, 720};

enum Action {
  FireGun,
  Up,
  Down,
  Left,
  Right,
  Select,
  Escape,
  Unused,
};

sf::Vector2f normalize_vector2f(const sf::Vector2f vector);
float vector2f_length(const sf::Vector2f vector);
sf::Vector2i screen_wrap(sf::Vector2i position);

}

#endif
