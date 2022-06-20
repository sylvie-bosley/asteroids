#ifndef ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_DISPLAY_MANAGER_H

#include <cmath>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "state_manager.h"

namespace ag {

class DisplayManager {
 public:
  DisplayManager();
  ~DisplayManager();

  bool load_resources(std::string game_font);
  sf::Vector2f screen_center() const;
  sf::Vector2f saucer_spawn_position() const;
  bool poll_event(sf::Event &event);
  void draw_screen(const StateManager &game_state, float dt,
                   const std::vector<std::shared_ptr<GameObject>> &objects,
                   unsigned int level);
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
      std::max(DISPLAY_SIZE.y / 10.0F, 40.0F)
    }
  };
  const sf::Vector2f TITLE_POSITION{DISPLAY_SIZE.x / 2.0F,
                                    DISPLAY_SIZE.y / 5.0F};
  const sf::Vector2f START_POSITION{DISPLAY_SIZE.x / 2.0F,
                                    2.0F * DISPLAY_SIZE.y / 3.0F};
  const sf::Vector2f LIFE_POSITION{10.0F, 10.0F};
  const sf::Vector2f LEVEL_POSITION{DISPLAY_SIZE.x / 2.0F, 10.0F};
  const sf::Vector2f SCORE_POSITION{DISPLAY_SIZE.x - 170.0F, 10.0F};
  const float BLINK_TIMER = 0.75F;

  sf::Text gameover_string() const;

  sf::RenderWindow m_game_window;
  sf::Font m_game_font;
  sf::ConvexShape m_life_sprite;
  sf::Text m_level_label;
  sf::Text m_score;
  sf::Text m_title_text;
  sf::Text m_press_enter;
  float m_blink_timer;
};

}

#endif