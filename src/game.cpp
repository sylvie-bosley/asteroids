#include "game.h"

#include <string>
#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "asteroid.h"
#include "saucer.h"
#include "collision_manager.h"
#include "display_manager.h"
#include "state_manager.h"

namespace ag {

Game::Game()
    : m_difficulty{0U}, m_next_object_id{0U}, m_saucer_timer{SAUCER_INTERVAL} {
  m_player = std::make_shared<Spaceship>(m_next_object_id++,
                                         m_display_manager.screen_center());
  m_game_objects.push_back(m_player);
  spawn_asteroids(STARTING_ASTEROIDS);
}

bool Game::load_resources(std::string title_bgm, std::string game_bgm,
                          std::string end_bgm, std::string ship_gun_sfx,
                          std::string game_font) {
  bool loaded = true;
#ifdef DEBUG
  if (!m_player->load_resources(ship_gun_sfx, game_font) ||
      !m_collision_manager.load_resources(ship_gun_sfx) ||
#else
  if (!m_player->load_resources(ship_gun_sfx) ||
#endif
      !m_game_state.load_resources(title_bgm, game_bgm, end_bgm) ||
      !m_game_font.loadFromFile(game_font)) {
    loaded = false;
  }
  return loaded;
}

bool Game::is_running() const {
  return m_game_state.is_running();
}

void Game::process_input(float dt) {
  sf::Event event;
  while (m_display_manager.poll_event(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        m_game_state.close_game();
        break;
      case sf::Event::LostFocus:
        if (m_game_state.in_game()) {
          m_game_state.pause_game();
        }
        break;
      case sf::Event::Resized:
        if (m_game_state.in_game()) {
          m_game_state.pause_game();
        }
        break;
      case sf::Event::KeyReleased:
        process_menu_keys(event.key.code);
        break;
      default:
        break;
    }
  }
  if (m_game_state.in_game()) {
    m_player->control_ship(dt);
  }
}

bool Game::update(float dt) {
  if (m_game_state.in_game()) {
    for (auto object : m_game_objects) {
      object->update(dt);
      if (*object == GameObject::SaucerType) {
        object->aim(m_player->get_position());
      }
    }
    std::vector<std::shared_ptr<GameObject>> new_objects;
    for (auto object : m_game_objects) {
      if (m_collision_manager.collision_check(*object, m_game_objects)) {
        object->destroy();
      }
      if ((*object == GameObject::PlayerType ||
           *object == GameObject::SaucerType) &&
          object->is_shooting()) {
        new_objects.push_back(object->spawn_child(m_next_object_id++));
      } else if (*object == GameObject::AsteroidType &&
                 object->is_destroyed() &&
                 object->get_radius() > S_ASTEROID) {
        new_objects.push_back(object->spawn_child(m_next_object_id++, 90.0F));
        new_objects.push_back(object->spawn_child(m_next_object_id++, -90.0F));
      }
      if (m_display_manager.off_camera(object->get_position(),
                                       object->get_radius())) {
        if (*object != GameObject::SaucerType) {
          m_display_manager.wrap_object(*object);
        } else {
          object->destroy();
        }
      }
    }
    if (m_saucer_timer <= 0.0F) {
      sf::Vector2f position = m_display_manager.saucer_spawn_position();
      float rotation = 0.0F;
      if (position.y > m_display_manager.screen_center().y) {
        rotation = 180.0F;
      }
      new_objects.push_back(std::make_shared<Saucer>(m_next_object_id++,
                                                     position, rotation));
      m_saucer_timer = SAUCER_INTERVAL;
    } else {
      m_saucer_timer -= dt;
    }
    m_game_objects.insert(m_game_objects.end(), new_objects.begin(),
                          new_objects.end());
    m_game_objects.erase(std::remove_copy_if(m_game_objects.begin() + 1U,
                                             m_game_objects.end(),
                                             m_game_objects.begin() + 1U,
                                             [](std::shared_ptr<GameObject> obj)
                                             { return obj->is_destroyed(); }),
                         m_game_objects.end());
  }
  if (m_player->is_destroyed()) {
    m_game_state.end_game();
  }
  return true;
}

void Game::render() {
  m_display_manager.clear_screen();
  if (m_game_state.game_over()) {
    sf::Text game_over_string = generate_game_over_string();
    m_display_manager.draw(&game_over_string);
  } else {
    for (auto object : m_game_objects) {
      m_display_manager.draw(object->get_sprite());
    }

#ifdef DEBUG
    m_display_manager.draw(m_player->get_ship_stats());
#endif
  }
  m_display_manager.render();
}

void Game::spawn_asteroids(unsigned int asteroid_count) {
  std::shared_ptr<Asteroid> new_asteroid;
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    new_asteroid = std::make_shared<Asteroid>(m_next_object_id++, L_ASTEROID,
        m_display_manager.valid_asteroid_position(m_game_objects),
        static_cast<float>(rand() % 360U));
    m_game_objects.push_back(new_asteroid);
  }
}

void Game::process_menu_keys(sf::Keyboard::Key key) {
  switch (m_game_state.state()) {
    case StateManager::TitleScreen:
      if (key == sf::Keyboard::Key::Enter) {
        m_game_state.start_game();
      } else if (key == sf::Keyboard::Key::Escape) {
        m_game_state.close_game();
      }
      break;
    case StateManager::InGame:
      if (key == sf::Keyboard::Key::Escape) {
        m_game_state.pause_game();
      }
      break;
    case StateManager::Paused:
      if (key == sf::Keyboard::Key::Escape) {
        reset_game();
      } else if (key == sf::Keyboard::Key::Enter) {
        m_game_state.resume_game();
      }
      break;
    case StateManager::GameOver:
      if (key == sf::Keyboard::Key::Enter) {
        reset_game();
      }
      break;
  }
}

void Game::reset_game() {
  m_difficulty = 0U;
  m_game_state.reset_state();
  m_player->reset_ship(m_display_manager.screen_center(), 0.0F,
                       sf::Vector2f{0.0F, 0.0F});
  m_game_objects.erase(m_game_objects.begin() + 1U, m_game_objects.end());
  m_next_object_id = static_cast<unsigned int>(m_game_objects.size());
  spawn_asteroids(STARTING_ASTEROIDS);
}

void Game::update_game_objects(float dt) {
}

void Game::delete_destroyed_objects() {
}

sf::Text Game::generate_game_over_string() const {
  sf::Text string{"GAME OVER", m_game_font, 100U};
  string.setFillColor(sf::Color::White);
  sf::Vector2f new_origin{string.getLocalBounds().width / 2.0F,
      string.getLocalBounds().height};
  string.setOrigin(new_origin);
  string.move(m_display_manager.screen_center());
  return string;
}

}
