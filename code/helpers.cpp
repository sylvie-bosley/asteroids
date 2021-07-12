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

sf::Vector2i screen_wrap(sf::Vector2i position) {
  if (position.x < 0) {
    position.x = DISPLAY_SIZE.x - 1;
  } else if (position.x > DISPLAY_SIZE.x) {
    position.x = 0;
  }
  if (position.y < 0) {
    position.y = DISPLAY_SIZE.y - 1;
  } else if (position.y > DISPLAY_SIZE.y) {
    position.y = 0;
  }
  return position;
}

}
