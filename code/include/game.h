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
  Game() {};
  Game(const sf::Font &font);
  ~Game() {};

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

  void generate_asteroids(const unsigned int asteroid_count);
  void reset_game();
  bool play_bgm();
  Action parse_player_action(const sf::Keyboard::Key key);

  sf::RenderWindow game_window;
  Spaceship player;
  sf::Music bgm;
  std::vector<Asteroid> asteroids;
  unsigned int starting_asteroids;
  unsigned int difficulty;
  GameState game_state;
  bool running;
};

}

#endif
