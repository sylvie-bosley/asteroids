#include "game.h"

#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "asteroid.h"
#include "display_manager.h"

namespace ag {

Game::Game()
    : m_game_window{sf::VideoMode(
                    static_cast<unsigned int>(m_display_manager.view_size().x),
                    static_cast<unsigned int>(m_display_manager.view_size().y)),
                    "Asteroids"},
      m_difficulty{0U}, m_running{true},
      m_game_state{TitleScreen} {
  m_player = std::make_shared<Spaceship>(m_display_manager.view_size() / 2.0F,
                                         0U);
  m_game_objects.push_back(m_player);
  m_next_object_id++;
  spawn_asteroids(STARTING_ASTEROIDS);
}

bool Game::load_resources(const std::string title_bgm,
                          const std::string game_bgm,
                          const std::string end_bgm,
                          const std::string ship_gun_sfx,
                          const std::string game_font) {
  bool loaded = true;
#ifdef DEBUG
  if (!m_player->load_resources(ship_gun_sfx, game_font) ||
      !m_collision_manager.load_resources(ship_gun_sfx) ||
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
          // pause_game();
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

bool Game::update(const float dt) {
  if (m_game_state == InGame) {
    sf::Vector2f wrapped_position;
    GameObject *collider;
    for (auto object : m_game_objects) {
      object->update(dt);
      m_collision_manager.collision_check(*object, m_game_objects);
      if (m_display_manager.off_camera(object->get_position(),
                                       object->get_radius())){
        m_display_manager.wrap_object(*object);
      }
    }
    std::vector<std::shared_ptr<GameObject>> new_children;
    for (auto object : m_game_objects) {
      if (object->get_object_type() == GameObject::AsteroidType &&
          object->is_destroyed() && object->get_radius() > S_ASTEROID) {
        new_children.push_back(object->spawn_child(*object, 90.0F,
                                                   m_next_object_id));
        m_next_object_id++;
        new_children.push_back(object->spawn_child(*object, -90.0F,
                                                   m_next_object_id));
        m_next_object_id++;
      }
    }
    m_game_objects.erase(std::remove_copy_if(m_game_objects.begin(),
      m_game_objects.end(), m_game_objects.begin(),
      [](std::shared_ptr<GameObject> object) {return object->is_destroyed();}),
      m_game_objects.end());
    m_game_objects.insert(m_game_objects.end(), new_children.begin(),
                          new_children.end());
  }
  if (m_player->is_destroyed()) {
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
    game_over_string.setPosition(m_display_manager.view_size() / 2.0F);
    m_game_window.draw(game_over_string);
  } else {
    for (auto object : m_game_objects) {
      m_game_window.draw(*object->get_sprite());
    }

#ifdef DEBUG
    m_game_window.draw(m_player->get_ship_stats());
#endif
  }
  m_game_window.display();
}

void Game::spawn_asteroids(const unsigned int asteroid_count) {
  std::shared_ptr<Asteroid> new_asteroid;
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    new_asteroid = std::make_shared<Asteroid>(L_ASTEROID, m_next_object_id,
        generate_valid_asteroid_position(), static_cast<float>(rand() % 360U));
    m_game_objects.push_back(new_asteroid);
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

const sf::Vector2f Game::generate_valid_asteroid_position() const {
  float old_x, old_y, new_x, new_y, distance;
  bool invalid;
  do {
    invalid = false;
    new_x = rand() % static_cast<int>(m_display_manager.view_size().x);
    new_y = rand() % static_cast<int>(m_display_manager.view_size().y);
    if (new_x <= 50.0F || new_x >= m_display_manager.view_size().x - 50.0F ||
        new_y <= 50.0F || new_y >= m_display_manager.view_size().y - 50.0F) {
      invalid = true;
    }
    for (auto object : m_game_objects) {
      old_x = object->get_position().x;
      old_y = object->get_position().y;
      distance = sqrt(pow((old_x - new_x), 2) + pow((old_y - new_y), 2));
      if (object->get_object_type() == GameObject::PlayerType &&
          distance < m_display_manager.view_size().y / 6.0F) {
          invalid = true;
      } else if (object->get_object_type() == GameObject::AsteroidType &&
                 distance < 110.0F) {
          invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
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
  m_player->reset_ship(m_display_manager.view_size() / 2.0F, 0.0F,
                         sf::Vector2f{0.0F, 0.0F});
  while (m_game_objects.size() > 1U) {
    m_game_objects.pop_back();
  }
  m_next_object_id = static_cast<unsigned int>(m_game_objects.size());
  spawn_asteroids(STARTING_ASTEROIDS);
}

void Game::close_game() {
  m_running = false;
  m_game_window.close();
}

}
