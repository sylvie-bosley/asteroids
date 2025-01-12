#include "helpers.hpp"

#include <cmath>

#include <SFML/System.hpp>

namespace ag {

sf::Vector2f normalize_vector2f(const sf::Vector2f &vector) {
  return vector / vector2f_length(vector);
}

float vector2f_length(const sf::Vector2f &vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float vector2f_dot_product(const sf::Vector2f &vector_one,
                           const sf::Vector2f &vector_two) {
  return (vector_one.x * vector_two.x) + (vector_one.y * vector_two.y);
}

}
