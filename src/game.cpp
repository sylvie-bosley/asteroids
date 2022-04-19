#include "game.h"

#include <cmath>
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
  spawn_asteroids(STARTING_ASTEROIDS, L_ASTEROID);
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
    for (std::shared_ptr<GameObject> object : m_game_objects) {
      object->update(dt);
    }
    int i = 0;
    while (i < m_game_objects.size()) {
      if (m_game_objects.at(i)->get_object_type() == GameObject::AsteroidType) {
        if (!m_game_objects.at(i)->has_wrapped()) {
          check_for_wrap(*m_game_objects.at(i));
        } else if (off_camera(m_game_objects.at(i)->get_position())) {
          m_game_objects.erase(remove(m_game_objects.begin(),
                                      m_game_objects.end(),
                                      m_game_objects.at(i)),
                               m_game_objects.end());
          i--;
        }
      }
      i++;
    }
    m_collision_manager.check_for_collisions(m_game_objects);
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

void Game::check_for_wrap(GameObject &object) {
  if (object.has_wrapped()) {
    return;
  }
  sf::Vector2f position = object.get_position();
  sf::Vector2f velocity = object.get_velocity();
  float wrapped_x = object.get_position().x;
  float wrapped_y = object.get_position().y;
  bool wrap_left = position.x <= 50.0F && velocity.x <= 0;
  bool wrap_right = position.x >= static_cast<float>(DISPLAY_SIZE.x - 50.0F) &&
                    velocity.x >= 0;
  bool wrap_top = position.y <= 50.0F && velocity.y <= 0;
  bool wrap_bottom = position.y >= static_cast<float>(DISPLAY_SIZE.y - 50.0F) &&
                     velocity.y >= 0;
  if (wrap_left) {
    wrapped_x = (position.x + static_cast<float>(DISPLAY_SIZE.x));
  } else if (wrap_right) {
    wrapped_x = (position.x - static_cast<float>(DISPLAY_SIZE.x));
  }

  if (wrap_top) {
    wrapped_y = (position.y + static_cast<float>(DISPLAY_SIZE.y));
  } else if (wrap_bottom) {
    wrapped_y = (position.y - static_cast<float>(DISPLAY_SIZE.y));
  }
  if (wrap_left || wrap_right || wrap_top || wrap_bottom) {
    m_game_objects.push_back(object.spawn_wrapped_copy(m_next_object_id,
        sf::Vector2f{wrapped_x, wrapped_y}));
    m_next_object_id++;
  }
  object.set_wrapped(wrap_left || wrap_right || wrap_top || wrap_bottom);
}

void Game::spawn_asteroids(const unsigned int asteroid_count,
                              const float size) {
  float direction, r_sin, r_cos;
  sf::Vector2f heading;
  std::shared_ptr<Asteroid> new_asteroid;
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    direction = static_cast<float>(rand() % 360U);
    r_sin = static_cast<float>(std::sin(direction * (M_PI / 180.0F)));
    r_cos = static_cast<float>(std::cos(direction * (M_PI / 180.0F)));
    heading.x = r_sin;
    heading.y = -r_cos;
    new_asteroid = std::make_shared<Asteroid>(size, m_next_object_id,
        generate_valid_asteroid_position(), (heading * ASTEROID_SPEED));
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
    new_x = static_cast<float>(rand() % DISPLAY_SIZE.x);
    new_y = static_cast<float>(rand() % DISPLAY_SIZE.y);
    if (new_x <= 50.0F || new_x >= DISPLAY_SIZE.x - 50.0F ||
        new_y <= 50.0F || new_y >= DISPLAY_SIZE.y - 50.0F) {
      invalid = true;
    }
    for (auto object : m_game_objects) {
      old_x = object->get_position().x;
      old_y = object->get_position().y;
      distance = sqrt(pow((old_x - new_x), 2) + pow((old_y - new_y), 2));
      if (object->get_object_type() == GameObject::PlayerType &&
          distance < static_cast<float>(DISPLAY_SIZE.y) / 6.0F) {
          invalid = true;
      } else if (object->get_object_type() == GameObject::AsteroidType &&
                 distance < 110.0F) {
          invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
}

const bool Game::off_camera(const sf::Vector2f position) const {
  return position.x < -50.0F || position.x > DISPLAY_SIZE.x + 50.0F ||
         position.y < -50.0F || position.y > DISPLAY_SIZE.y + 50.0F;
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
  spawn_asteroids(STARTING_ASTEROIDS, L_ASTEROID);
}

void Game::close_game() {
  m_running = false;
  m_game_window.close();
}

}
