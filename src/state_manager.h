#ifndef ASTEROIDS_GAME_CODE_INCLUDE_STATE_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_STATE_MANAGER_H

#include <string>

#include <SFML/Audio.hpp>

namespace ag {

class StateManager {
 public:
  enum GameState {
    TitleScreen,
    InGame,
    Paused,
    GameOver
  };

  StateManager();
  ~StateManager() {};

  bool load_resources(std::string title_bgm, std::string game_bgm,
                      std::string end_bgm);
  GameState state() const;
  bool is_running() const;
  bool in_game() const;
  bool game_over() const;
  void start_game();
  void pause_game();
  void resume_game();
  void end_game();
  void reset_state ();
  void close_game();

 private:
  GameState m_state;
  bool m_running;
  sf::Music m_title_bgm;
  sf::Music m_game_bgm;
  sf::Music m_end_bgm;
};

}

#endif
