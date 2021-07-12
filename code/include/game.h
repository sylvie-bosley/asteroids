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

  Action process_input();
  bool update(const Action action, const sf::Time dt);
  void render();

  bool running;

 private:
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  Spaceship player;
  sf::RenderWindow game_window;
  sf::Music bgm;

  void generate_asteroids(const unsigned int current_asteroid_count);
  void start_game();
  void pause_game();
  void resume_game();
  void reset_game();
  void close_game();
  bool play_bgm();
  Action parse_player_action(const sf::Keyboard::Key key);

  std::vector<Asteroid> asteroids;
  unsigned int starting_asteroids;
  unsigned int difficulty;
  GameState game_state;
};

}

#endif
