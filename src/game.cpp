#include "game.h"

#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "asteroid.h"

namespace ag {

Game::Game()
    : m_game_window{sf::VideoMode(DISPLAY_SIZE.x, DISPLAY_SIZE.y), "Asteroids"},
      m_next_object_id{1U}, m_difficulty{0U}, m_running{true},
      m_game_state{TitleScreen} {
  m_player = std::make_shared<Spaceship>(
      static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U), 0U);
  m_game_objects.push_back(m_player);
  generate_asteroids(STARTING_ASTEROIDS, L_ASTEROID);
}

bool Game::load_resources(const std::string title_bgm,
                          const std::string game_bgm,
                          const std::string end_bgm,
                          const std::string ship_gun_sfx,
                          const std::string game_font) {
  bool loaded = true;
#ifdef DEBUG
  if (!m_player->load_resources(ship_gun_sfx, game_font) ||
#else
  if (!m_player->load_resources(ship_gun_sfx) ||
#endif
      !m_title_bgm.openFromFile(title_bgm) ||
      !m_game_bgm.openFromFile(game_bgm) ||
      !m_end_bgm.openFromFile(end_bgm) ||
      !m_game_font.loadFromFile(game_font)) {
    loaded = false;
  } else {
    m_title_bgm.setLoop(true);
    m_game_bgm.setLoop(true);
    m_end_bgm.setLoop(true);
    m_title_bgm.play();
  }
  return loaded;
}

bool Game::is_running() const {
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
    m_player->control_ship();
  }
}

bool Game::update(const sf::Time dt) {
  if (m_game_state == InGame) {
    for (unsigned int i = 0U; i < m_game_objects.size(); ++i) {
      m_game_objects.at(i)->update(dt);
    }
    m_collision_manager.check_for_collisions(m_game_objects);
  }
  if (m_player->destroyed()) {
    game_over();
  }
  return true;
}

void Game::render() {
  m_game_window.clear(sf::Color::Black);
  if (m_game_state == GameOver) {
    sf::Text game_over_string{"GAME OVER", m_game_font, 100U};
    game_over_string.setFillColor(sf::Color::White);
    sf::Vector2f new_origin{game_over_string.getLocalBounds().width / 2.0F,
        game_over_string.getLocalBounds().height};
    game_over_string.setOrigin(new_origin);
    game_over_string.setPosition(static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U));
    m_game_window.draw(game_over_string);
  } else {
    for (unsigned int i = 0U; i < m_game_objects.size(); ++i) {
      m_game_window.draw(*m_game_objects.at(i)->get_sprite());
    }

#ifdef DEBUG
    m_game_window.draw(m_player->get_ship_stats());
#endif
  }
  m_game_window.display();
}

void Game::generate_asteroids(const unsigned int asteroid_count,
                              const float size) {
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    m_game_objects.push_back(std::make_shared<Asteroid>(size,
                                                        m_next_object_id,
                                                        m_game_objects));
    m_next_object_id++;
  }
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
  m_difficulty = 0U;
  m_game_state = TitleScreen;
  m_title_bgm.play();
  m_game_bgm.setVolume(100.0F);
  m_player->reset_ship(static_cast<sf::Vector2f>(DISPLAY_SIZE / 2U), 0.0F,
                         sf::Vector2f{0.0F, 0.0F});
  while (m_game_objects.size() > 1U) {
    m_game_objects.pop_back();
  }
  m_next_object_id = static_cast<unsigned int>(m_game_objects.size());
  generate_asteroids(STARTING_ASTEROIDS, L_ASTEROID);
}

void Game::close_game() {
  m_running = false;
  m_game_window.close();
}

}
