#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "collision_manager.h"
#include "display_manager.h"
#include "state_manager.h"

namespace ag {

class Game {
 public:
  Game();
  ~Game() {};

  bool load_resources(std::string title_bgm, std::string game_bgm,
                      std::string end_bgm, std::string ship_gun_sfx,
                      std::string game_font);
  bool is_running() const;
  void process_input();
  bool update(float dt);
  void render();

 private:
  unsigned int STARTING_ASTEROIDS = 10U;
  float L_ASTEROID = 50.0F;
  float M_ASTEROID = 25.0F;
  float S_ASTEROID = 12.5F;

  void spawn_asteroids(unsigned int asteroid_count);
  void process_menu_keys(sf::Keyboard::Key key);
  void reset_game();
  void update_game_objects(float dt);
  void spawn_child_asteroids(float dt);
  void delete_destroyed_objects(float dt);
  sf::Text generate_game_over_string() const;

  StateManager m_game_state;
  CollisionManager m_collision_manager;
  DisplayManager m_display_manager;
  std::vector<std::shared_ptr<GameObject>> m_game_objects;
  std::shared_ptr<Spaceship> m_player;
  unsigned int m_next_object_id = 0U;
  sf::Font m_game_font;
  unsigned int m_difficulty;
};

}

#endif
