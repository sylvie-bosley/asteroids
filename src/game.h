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

  bool load_resources(std::string game_bgm, std::string ship_gun_sfx,
                      std::string game_font);
  bool is_running() const;
  void process_input(float dt);
  bool update(float dt);
  void render(float dt);

 private:
  const unsigned int STARTING_ASTEROIDS = 3U;
  const float L_ASTEROID = 50.0F;
  const float M_ASTEROID = 25.0F;
  const float S_ASTEROID = 12.5F;
  const float SAUCER_INTERVAL = 30.0F;

  void spawn_asteroids(unsigned int asteroid_count);
  void reset_game();

  StateManager m_game_state;
  CollisionManager m_collision_manager;
  DisplayManager m_display_manager;
  std::vector<std::shared_ptr<GameObject>> m_game_objects;
  std::shared_ptr<Spaceship> m_player;
  unsigned int m_next_object_id;
  unsigned int m_difficulty;
  float m_saucer_timer;
};

}

#endif
