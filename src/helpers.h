#ifndef ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H
#define ASTEROIDS_GAME_CODE_INCLUDE_HELPERS_H

#include <SFML/System.hpp>

namespace ag {

sf::Vector2f normalize_vector2f(const sf::Vector2f &vector);
float vector2f_length(const sf::Vector2f &vector);

}

#endif
