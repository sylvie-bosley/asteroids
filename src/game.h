#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "collision_manager.h"

namespace ag {

class Game {
 public:
  Game();
  ~Game() {};

  bool load_resources(const std::string title_bgm, const std::string game_bgm,
                      const std::string end_bgm, const std::string ship_gun_sfx,
                      const std::string game_font);
  bool is_running() const;
  void process_input();
  bool update(const sf::Time dt);
  void render();

 private:
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  const unsigned int STARTING_ASTEROIDS = 20U;
  const float ASTEROID_SPEED = 25.0F;
  const float L_ASTEROID = 50.0F;
  const float M_ASTEROID = 25.0F;
  const float S_ASTEROID = 12.5F;

  void check_for_wrap(GameObject &object);
  void spawn_asteroids(const unsigned int asteroid_count, const float size);
  void process_menu_keys(const sf::Keyboard::Key key);
  const sf::Vector2f generate_valid_asteroid_position() const;
  const bool off_camera(const sf::Vector2f position) const;
  void start_game();
  void pause_game();
  void resume_game();
  void game_over();
  void reset_game();
  void close_game();

  sf::RenderWindow m_game_window;
  std::vector<std::shared_ptr<GameObject>> m_game_objects;
  std::shared_ptr<Spaceship> m_player;
  unsigned int m_next_object_id = 0U;
  CollisionManager m_collision_manager;
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
