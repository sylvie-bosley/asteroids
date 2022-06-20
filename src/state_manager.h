#ifndef ASTEROIDS_GAME_CODE_INCLUDE_STATE_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_STATE_MANAGER_H

#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ag {

class StateManager {
 public:
  enum GameState {
    TitleScreen,
    LoadGame,
    InGame,
    Paused,
    GameOver,
    Reset
  };

  StateManager();
  ~StateManager() {};

  bool load_resources(std::string game_bgm);
  bool is_running() const;
  bool title_screen() const;
  bool load() const;
  bool in_game() const;
  bool paused() const;
  bool game_over() const;
  bool reset() const;
  void update_game_state(sf::Keyboard::Key key);
  void start_game();
  void pause_game();
  void next_level();
  void end_game();
  void reset_game_state ();
  void close_game();

 private:
  GameState m_state;
  bool m_running;
  sf::Music m_game_bgm;
};

}

#endif
