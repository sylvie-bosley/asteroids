#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include "spaceship.h"
#include "helpers.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ag {

class Game {
 public:
  Game();
  ~Game();

  void pause_game();
  void resume();
  Action process_input();
  bool update(const Action action, const sf::Time dt);
  void render();
  void close_game();

  bool running;

 private:
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  const sf::Vector2i dspl_size{1280, 720};

  Spaceship player;
  sf::RenderWindow game_window;
  sf::Music bgm;

  bool play_bgm();
  Action parse_player_action(const sf::Keyboard::Key key);

  GameState game_state;
};

}

#endif
