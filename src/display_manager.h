#ifndef ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H

#include <SFML/Graphics.hpp>

#include "game_object.h"

namespace ag {

class DisplayManager {
 public:
  DisplayManager() {};
  ~DisplayManager() {};

  const sf::Vector2f view_size() const;
  void wrap_object(GameObject &object);
  const bool off_camera(const sf::Vector2f position, const float radius) const;

 private:
  const sf::Vector2f DISPLAY_SIZE{1280.0F, 720.0F};
};

}

#endif