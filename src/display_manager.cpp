#include "display_manager.h"

#include <SFML/Graphics.hpp>

namespace ag{

const sf::Vector2f DisplayManager::view_size() const {
  return DISPLAY_SIZE;
}

void DisplayManager::wrap_object(GameObject &object) {
  float wrapped_x = object.get_position().x;
  float wrapped_y = object.get_position().y;
  if (object.get_position().x <= 0.0F) {
    wrapped_x = (object.get_position().x + DISPLAY_SIZE.x +
                 (object.get_radius() * 2.0F));
  } else if (object.get_position().x >= DISPLAY_SIZE.x) {
    wrapped_x = (object.get_position().x - DISPLAY_SIZE.x -
                 (object.get_radius() * 2.0F));
  }
  if (object.get_position().y <= 0.0F) {
    wrapped_y = (object.get_position().y + DISPLAY_SIZE.y +
                 (object.get_radius() * 2.0F));
  } else if (object.get_position().y >= DISPLAY_SIZE.y) {
    wrapped_y = (object.get_position().y - DISPLAY_SIZE.y -
                 (object.get_radius() * 2.0F));
  }
  object.move_to(sf::Vector2f{wrapped_x, wrapped_y});
}

const bool DisplayManager::off_camera(const sf::Vector2f position,
                                      const float radius) const {
  return position.x < -radius ||
         position.y < -radius ||
         position.x > DISPLAY_SIZE.x + radius ||
         position.y > DISPLAY_SIZE.y + radius;
}

}