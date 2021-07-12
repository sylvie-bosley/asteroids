#ifndef ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H
#define ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H

#include <SFML/System.hpp>

namespace ag {

const sf::Vector2u DISPLAY_SIZE{1280U, 720U};

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
sf::Vector2f screen_wrap(const sf::Vector2f position);

}

#endif
