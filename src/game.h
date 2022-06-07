#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "collision_manager.h"
#include "display_manager.h"

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
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  unsigned int STARTING_ASTEROIDS = 10U;
  float L_ASTEROID = 50.0F;
  float M_ASTEROID = 25.0F;
  float S_ASTEROID = 12.5F;

  void spawn_asteroids(unsigned int asteroid_count);
  void process_menu_keys(sf::Keyboard::Key key);
  sf::Vector2f generate_valid_asteroid_position() const;
  bool off_camera(sf::Vector2f position) const;
  void start_game();
  void pause_game();
  void resume_game();
  void game_over();
  void reset_game();
  void close_game();
  void update_game_objects(float dt);
  void spawn_child_asteroids(float dt);
  void delete_destroyed_objects(float dt);
  sf::Text game_over_string() const;

  CollisionManager m_collision_manager;
  DisplayManager m_display_manager;
  sf::RenderWindow m_game_window;
  std::vector<std::shared_ptr<GameObject>> m_game_objects;
  std::shared_ptr<Spaceship> m_player;
  unsigned int m_next_object_id = 0U;
  sf::Music m_title_bgm;
  sf::Music m_game_bgm;
  sf::Music m_end_bgm;
  sf::Font m_game_font;
  unsigned int m_difficulty;
  GameState m_game_state;
  bool m_running;
};

}

#endif
