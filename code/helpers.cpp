#include "include/helpers.h"

#include <SFML/System.hpp>
#include <cmath>

namespace ag {

sf::Vector2f normalize_vector2f(const sf::Vector2f vector)
{
  sf::Vector2f normal_vector;
  float length = vector2f_length(vector);
  normal_vector.x = vector.x / length;
  normal_vector.y = vector.y / length;
  return normal_vector;
}

float vector2f_length(const sf::Vector2f vector)
{
  float length = sqrt(vector.x * vector.x + vector.y * vector.y);
  return length;
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
