#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "asteroid.h"
#include "spaceship.h"
#include "helpers.h"

namespace ag {

class Game {
 public:
  Game();
  ~Game() {};

  bool load_resources(
      std::string title_bgm, std::string game_bgm, std::string end_bgm,
      std::string ship_gun_sfx, std::string ship_font);
  bool is_running();
  Action process_input();
  bool update(const Action action, const sf::Time &dt);
  void render();

 private:
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  const unsigned int STARTING_ASTEROIDS = 4U;

  void generate_asteroids(const unsigned int asteroid_count, const float size);
  void reset_game();
  void update_bgm();
  Action parse_player_action(const sf::Keyboard::Key key);

  sf::RenderWindow m_game_window;
  Spaceship m_player;
  sf::Music m_title_bgm;
  sf::Music m_game_bgm;
  sf::Music m_end_bgm;
  std::vector<Asteroid> m_asteroids;
  unsigned int m_difficulty;
  GameState m_game_state;
  bool m_running;
};

}

#endif
