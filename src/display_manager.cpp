#include "display_manager.h"

#include <cmath>

#include <SFML/Graphics.hpp>

namespace ag{

sf::Vector2f DisplayManager::view_size() const {
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

bool DisplayManager::off_camera(sf::Vector2f position, float radius) const {
  return position.x < -radius ||
         position.y < -radius ||
         position.x > DISPLAY_SIZE.x + radius ||
         position.y > DISPLAY_SIZE.y + radius;
}

sf::Vector2f DisplayManager::valid_asteroid_position(
    const std::vector<std::shared_ptr<GameObject>> &game_objects) const {
  float old_x, old_y, new_x, new_y, distance;
  bool invalid;
  do {
    invalid = false;
    new_x = rand() % static_cast<int>(view_size().x);
    new_y = rand() % static_cast<int>(view_size().y);
    if (new_x <= 50.0F || new_x >= view_size().x - 50.0F ||
        new_y <= 50.0F || new_y >= view_size().y - 50.0F) {
      invalid = true;
    }
    for (auto object : game_objects) {
      old_x = object->get_position().x;
      old_y = object->get_position().y;
      distance = sqrt(pow((old_x - new_x), 2) + pow((old_y - new_y), 2));
      if (object->get_object_type() == GameObject::PlayerType &&
          distance < view_size().y / 6.0F) {
          invalid = true;
      } else if (object->get_object_type() == GameObject::AsteroidType &&
                 distance < 110.0F) {
          invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
}

}