#ifndef ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H

#include <SFML/Graphics.hpp>

#include "game_object.h"

namespace ag {

class DisplayManager {
 public:
  DisplayManager();
  ~DisplayManager();

  sf::Vector2f player_spawn() const;
  sf::Vector2f screen_center() const;
  bool poll_event(sf::Event &event);
  void clear_screen();
  void draw(const sf::Drawable *object);
  void render();
  void wrap_object(GameObject &object);
  bool off_camera(sf::Vector2f position, float radius) const;
  sf::Vector2f valid_asteroid_position(
    const std::vector<std::shared_ptr<GameObject>> &game_objects) const;

 private:
  const sf::Vector2f DISPLAY_SIZE{1280.0F, 720.0F};

  sf::RenderWindow m_game_window;
};

}

#endif