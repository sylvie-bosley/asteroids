#include "helpers.h"

#include <cmath>

#include <SFML/System.hpp>

namespace ag {

sf::Vector2f normalize_vector2f(const sf::Vector2f &vector) {
  return vector / vector2f_length(vector);
}

float vector2f_length(const sf::Vector2f &vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

}
