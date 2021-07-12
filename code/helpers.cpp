#include "include/helpers.h"

#include <cmath>

#include <SFML/System.hpp>

namespace ag {

sf::Vector2f normalize_vector2f(const sf::Vector2f vector) {
  return vector / vector2f_length(vector);
}

float vector2f_length(const sf::Vector2f vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f screen_wrap(const sf::Vector2f position) {
  sf::Vector2f wrapped_position = position;
  if (wrapped_position.x < 0.0F) {
    wrapped_position.x = static_cast<float>(DISPLAY_SIZE.x - 1U);
  } else if (wrapped_position.x > DISPLAY_SIZE.x) {
    wrapped_position.x = 0.0F;
  }
  if (wrapped_position.y < 0.0F) {
    wrapped_position.y = static_cast<float>(DISPLAY_SIZE.y - 1U);
  } else if (wrapped_position.y > DISPLAY_SIZE.y) {
    wrapped_position.y = 0.0F;
  }
  return wrapped_position;
}

}
