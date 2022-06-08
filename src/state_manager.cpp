#include "state_manager.h"

#include <string>

#include <SFML/Audio.hpp>

namespace ag {

StateManager::StateManager() : m_state{TitleScreen}, m_running{true} {}

bool StateManager::load_resources(std::string title_bgm, std::string game_bgm,
                                  std::string end_bgm) {
  bool loaded = true;
  if (!m_title_bgm.openFromFile(title_bgm) ||
      !m_game_bgm.openFromFile(game_bgm) ||
      !m_end_bgm.openFromFile(end_bgm)) {
    loaded - false;
  } else {
    m_title_bgm.setLoop(true);
    m_game_bgm.setLoop(true);
    m_end_bgm.setLoop(true);
    m_title_bgm.play();
  }
  return loaded;
}

StateManager::GameState StateManager::state() const {
  return m_state;
}

bool StateManager::is_running() const {
  return m_running;
}

bool StateManager::in_game() const {
  return m_state == InGame;
}

bool StateManager::game_over() const {
  return m_state == GameOver;
}

void StateManager::start_game() {
  m_state = StateManager::InGame;
  m_title_bgm.stop();
  m_game_bgm.play();
}

void StateManager::pause_game() {
  m_state = StateManager::Paused;
  m_game_bgm.setVolume(25.0F);
}

void StateManager::resume_game() {
  m_state = StateManager::InGame;
  m_game_bgm.setVolume(100.0F);
}

void StateManager::end_game() {
  m_state = StateManager::GameOver;
  m_game_bgm.stop();
  m_end_bgm.play();
}

void StateManager::reset_state() {
  m_state = TitleScreen;
  m_title_bgm.play();
  m_game_bgm.setVolume(100.0F);
}

void StateManager::close_game() {
  m_running = false;
}

}
