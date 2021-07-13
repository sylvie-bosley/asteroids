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

Action Game::process_input() {
  Action action;
  sf::Event event;
  while (m_game_window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      m_running = false;
      m_game_window.close();
      break;
    case sf::Event::LostFocus:
      m_game_state = Paused;
      break;
    case sf::Event::Resized:
      m_game_state = Paused;
      break;
    case sf::Event::KeyPressed:
      action = parse_player_action(event.key.code);
      break;
    default:
      break;
    }
  }
  return action;
}

bool Game::update(const Action action, const sf::Time &dt) {
  switch (m_game_state) {
    case TitleScreen: {
      if (action == Enter) {
        m_title_bgm.stop();
        m_game_bgm.play();
        m_game_state = InGame;
      } else if (action == Escape) {
        m_title_bgm.stop();
        m_running = false;
        m_game_window.close();
      }
      break;
    }
    case InGame: {
      if (action == Escape) {
        m_game_bgm.setVolume(25.0F);
        m_game_state = Paused;
      } else if (action != Enter && action != Unused) {
        m_player.control_ship(action, dt);
      }
      m_player.update();
      for (unsigned int i = 0U; i < (STARTING_ASTEROIDS + m_difficulty); ++i) {
        m_asteroids[i].update(dt);
      }
      if (/*player dies*/false) {
        m_game_bgm.stop();
        m_end_bgm.play();
        m_game_state = GameOver;
      }
      break;
    }
    case Paused: {
      if (action == Enter) {
        m_game_bgm.setVolume(100.0F);
        m_game_state = InGame;
      } else if (action == Escape) {
        m_game_bgm.setVolume(100.0F);
        m_game_bgm.stop();
        reset_game();
      }
      break;
    }
    case GameOver: {
      if (action == Enter) {
        m_end_bgm.stop();
        reset_game();
      }
      break;
    }
  }
  return true;
}

void Game::render() {
  m_game_window.clear(sf::Color::Black);
  m_game_window.draw(m_player.get_sprite());
  for (unsigned int i = 0U; i < (STARTING_ASTEROIDS + m_difficulty); ++i) {
    m_game_window.draw(m_asteroids[i].get_sprite());
  }
  m_game_window.draw(m_player.get_ship_stats());
  m_game_window.display();
}

void Game::generate_asteroids(
    const unsigned int asteroid_count, const float size) {
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    m_asteroids.push_back(Asteroid{size});
  }
}

void Game::reset_game() {
  m_game_state = TitleScreen;
  m_title_bgm.play();
  m_player.reset_ship();
  m_asteroids.clear();
  generate_asteroids(STARTING_ASTEROIDS, 50.0F);
}

void Game::update_bgm() {
  switch (m_game_state) {
    case TitleScreen:
      if (m_game_bgm.getStatus() == sf::Sound::Playing) {
        m_game_bgm.stop();
      } else if (m_end_bgm.getStatus() == sf::Sound::Playing) {
        m_end_bgm.stop();
      }
      if (m_title_bgm.getStatus() != sf::Sound::Playing) {
        m_title_bgm.play();
      }
      break;
    case InGame:
      m_title_bgm.stop();
      m_end_bgm.stop();
      m_game_bgm.setVolume(100.0F);
      if (m_game_bgm.getStatus() != sf::Sound::Playing) {
        m_game_bgm.play();
      }
      break;
    case Paused:
      m_title_bgm.stop();
      m_end_bgm.stop();
      m_game_bgm.setVolume(25.0F);
      if (m_game_bgm.getStatus() != sf::Sound::Playing) {
        m_game_bgm.play();
      }
      break;
    case GameOver:
      m_title_bgm.stop();
      m_game_bgm.stop();
      if (m_end_bgm.getStatus() != sf::Sound::Playing) {
        m_end_bgm.play();
      }
      break;
  }
}

Action Game::parse_player_action(const sf::Keyboard::Key key) {
  Action action;
  switch (key) {
    case sf::Keyboard::Key::Escape:
      action = Escape;
      break;
    case sf::Keyboard::Key::Enter:
      action = Enter;
      break;
    case sf::Keyboard::Key::Space:
      action = Space;
      break;
    case sf::Keyboard::Key::Up:
      action = Up;
      break;
    case sf::Keyboard::Key::Down:
      action = Down;
      break;
    case sf::Keyboard::Key::Left:
      action = Left;
      break;
    case sf::Keyboard::Key::Right:
      action = Right;
      break;
    default:
      action = Unused;
      break;
  }
  return action;
}

}
