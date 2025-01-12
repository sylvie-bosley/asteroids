#include "state_manager.hpp"

#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ag {

StateManager::StateManager() : m_state{TitleScreen}, m_running{true} {}

bool StateManager::load_resources(std::string game_bgm) {
  if (!m_game_bgm.openFromFile(game_bgm)) {
    return false;
  } else {
    m_game_bgm.setLoop(true);
  }
  return true;
}

bool StateManager::is_running() const {
  return m_running;
}

bool StateManager::title_screen() const {
  return m_state == TitleScreen;
}

bool StateManager::load() const {
  return m_state == LoadGame;
}

bool StateManager::in_game() const {
  return m_state == InGame;
}

bool StateManager::paused() const {
  return m_state == Paused;
}

bool StateManager::game_over() const {
  return m_state == GameOver;
}

bool StateManager::reset() const {
  return m_state == Reset;
}

void StateManager::update_game_state(sf::Keyboard::Key key) {
  switch (m_state) {
    case StateManager::TitleScreen:
      if (key == sf::Keyboard::Key::Enter) {
        m_state = StateManager::LoadGame;
      } else if (key == sf::Keyboard::Key::Escape) {
        m_running = false;
      }
      break;
    case StateManager::InGame:
      if (key == sf::Keyboard::Key::Escape) {
        m_state = StateManager::Paused;
        m_game_bgm.setVolume(25.0F);
      }
      break;
    case StateManager::Paused:
      if (key == sf::Keyboard::Key::Escape) {
        m_state = StateManager::Reset;
        m_game_bgm.stop();
      } else if (key == sf::Keyboard::Key::Enter) {
        m_state = StateManager::InGame;
        m_game_bgm.setVolume(100.0F);
      }
      break;
    case StateManager::GameOver:
      if (key == sf::Keyboard::Key::Enter) {
        m_state = StateManager::Reset;
      }
      break;
  }
}

void StateManager::start_game() {
  m_state = StateManager::InGame;
  m_game_bgm.play();
}

void StateManager::pause_game() {
  m_state = StateManager::Paused;
  m_game_bgm.setVolume(25.0F);
}

void StateManager::next_level() {
  m_state = StateManager::LoadGame;
  m_game_bgm.stop();
}

void StateManager::end_game() {
  m_state = StateManager::GameOver;
  m_game_bgm.stop();
}

void StateManager::reset_game_state() {
  m_state = TitleScreen;
  m_game_bgm.setVolume(100.0F);
}

void StateManager::close_game() {
  m_running = false;
}

}
