#include "include/game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/asteroid.h"
#include "include/spaceship.h"
#include "include/helpers.h"

namespace ag {

Game::Game()
    : m_game_window{sf::VideoMode(DISPLAY_SIZE.x, DISPLAY_SIZE.y), "Asteroids"},
      m_player{} {
  m_difficulty = 0U;
  generate_asteroids(STARTING_ASTEROIDS, 50.0F);
  m_running = true;
  m_game_state = TitleScreen;
}

bool Game::load_resources(std::string title_bgm, std::string game_bgm,
    std::string end_bgm, std::string ship_gun_sfx, std::string ship_font) {
  bool loaded = true;
  if (!m_player.load_resources(ship_gun_sfx, ship_font) ||
      !m_title_bgm.openFromFile(title_bgm) ||
      !m_game_bgm.openFromFile(game_bgm) ||
      !m_end_bgm.openFromFile(end_bgm)) {
    loaded = false;
  } else {
    m_title_bgm.setLoop(true);
    m_game_bgm.setLoop(true);
    m_end_bgm.setLoop(true);
    m_title_bgm.play();
  }
  return loaded;
}

bool Game::is_running() {
  return m_running;
}

void Game::process_input() {
  sf::Event event;
  while (m_game_window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        close_game();
        break;
      case sf::Event::LostFocus:
        if (m_game_state == InGame) {
          pause_game();
        }
        break;
      case sf::Event::Resized:
        if (m_game_state == InGame) {
          pause_game();
        }
        break;
      case sf::Event::KeyReleased:
        process_menu_keys(event.key.code);
        break;
      default:
        break;
    }
  }
  if (m_game_state == InGame) {
    m_player.control_ship();
  }
}

bool Game::update(const sf::Time &dt) {
  if (m_game_state == InGame) {
    m_player.update(dt);
    for (unsigned int i = 0U; i < (STARTING_ASTEROIDS + m_difficulty); ++i) {
      m_asteroids[i].update(dt);
    }
    if (/* TODO: check for collision */false) {
      game_over();
    }
  }
  return true;
}

void Game::render() {
  m_game_window.clear(sf::Color::Black);
  if (m_game_state == GameOver) {
    // TODO: Game over stuff
  } else {
    m_game_window.draw(m_player.get_sprite());
    for (unsigned int i = 0U; i < (STARTING_ASTEROIDS + m_difficulty); ++i) {
      m_game_window.draw(m_asteroids[i].get_sprite());
    }

    // DEBUG
    m_game_window.draw(m_player.get_ship_stats());
    // END DEBUG
  }
  m_game_window.display();
}

void Game::process_menu_keys(const sf::Keyboard::Key key) {
  switch (m_game_state) {
    case TitleScreen:
      if (key == sf::Keyboard::Key::Enter) {
        start_game();
      } else if (key == sf::Keyboard::Key::Escape) {
        close_game();
      }
      break;
    case InGame:
      if (key == sf::Keyboard::Key::Escape) {
        pause_game();
      }
      break;
    case Paused:
      if (key == sf::Keyboard::Key::Escape) {
        reset_game();
      } else if (key == sf::Keyboard::Key::Enter) {
        resume_game();
      }
      break;
    case GameOver:
      if (key == sf::Keyboard::Key::Enter) {
        reset_game();
      }
      break;
  }
}

void Game::generate_asteroids(const unsigned int asteroid_count,
    const float size) {
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    m_asteroids.push_back(Asteroid{size});
  }
}

void Game::start_game() {
  m_game_state = InGame;
  m_title_bgm.stop();
  m_game_bgm.play();
}

void Game::pause_game() {
  m_game_state = Paused;
  m_game_bgm.setVolume(25.0F);
}

void Game::resume_game() {
  m_game_state = InGame;
  m_game_bgm.setVolume(100.0F);
}

void Game::game_over() {
  m_game_state = GameOver;
  m_game_bgm.stop();
  m_end_bgm.play();
}

void Game::reset_game() {
  m_game_state = TitleScreen;
  m_title_bgm.play();
  m_game_bgm.setVolume(100.0F);
  m_player.reset_ship();
  m_asteroids.clear();
  generate_asteroids(STARTING_ASTEROIDS, 50.0F);
}

void Game::close_game() {
  m_running = false;
  m_game_window.close();
}

}
