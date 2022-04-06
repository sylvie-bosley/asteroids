#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "asteroid.h"
#include "spaceship.h"
#include "quadtree.h"

namespace ag {

class Game {
 public:
  Game();
  ~Game() {};

  bool load_resources(const std::string title_bgm, const std::string game_bgm,
                      const std::string end_bgm, const std::string ship_gun_sfx,
                      const std::string ship_font);
  bool is_running();
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

  const unsigned int STARTING_ASTEROIDS = 4U;

  void process_menu_keys(const sf::Keyboard::Key key);
  void generate_asteroids(const unsigned int asteroid_count, const float size);
  void start_game();
  void pause_game();
  void resume_game();
  void game_over();
  void reset_game();
  void close_game();

  sf::RenderWindow m_game_window;
  Spaceship m_player;
  sf::Music m_title_bgm;
  sf::Music m_game_bgm;
  sf::Music m_end_bgm;
  sf::Font m_game_font;
  std::vector<Asteroid> m_asteroids;
  unsigned int m_difficulty;
  GameState m_game_state;
  bool m_running;
  unsigned int next_object_id = 0;
};

}

#endif
