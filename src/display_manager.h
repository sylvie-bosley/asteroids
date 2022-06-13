#ifndef ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H

#include <SFML/Graphics.hpp>

#include <cmath>

#include "game_object.h"

namespace ag {

class DisplayManager {
 public:
  DisplayManager();
  ~DisplayManager();

  sf::Vector2f screen_center() const;
  sf::Vector2f saucer_spawn_position() const;
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
  const std::vector<sf::Vector2f> SAUCER_SPAWNS{
    sf::Vector2f{
      -10.0F,
      std::min(DISPLAY_SIZE.y - DISPLAY_SIZE.y / 10.0F, DISPLAY_SIZE.y - 10.0F)
    },
    sf::Vector2f{
      DISPLAY_SIZE.x + 10.0F,
      std::max(DISPLAY_SIZE.y / 10.0F, 10.0F)
    }
  };

  sf::RenderWindow m_game_window;
};

}

#endif