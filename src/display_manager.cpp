#include "display_manager.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>

#include "game_object.hpp"
#include "spaceship.hpp"
#include "state_manager.hpp"

namespace ag{

DisplayManager::DisplayManager ()
  : m_game_window{sf::VideoMode(static_cast<unsigned int>(DISPLAY_SIZE.x),
                                static_cast<unsigned int>(DISPLAY_SIZE.y)),
                  "Asteroids"}, m_life_sprite{3U}, m_blink_timer{BLINK_TIMER} {
  m_life_sprite.setPointCount(3);
  m_life_sprite.setPoint(std::size_t(0U), sf::Vector2f{7.50F, 0.0F});
  m_life_sprite.setPoint(std::size_t(1U), sf::Vector2f{0.0F, 20.0F});
  m_life_sprite.setPoint(std::size_t(2U), sf::Vector2f{15.0F, 20.0F});
  m_life_sprite.setOutlineThickness(1.0F);
  m_life_sprite.setFillColor(sf::Color::Black);
  m_level_label.setCharacterSize(20U);
  m_level_label.setString("LEVEL 1");
  m_level_label.setFillColor(sf::Color::White);
  m_score.setCharacterSize(20U);
  m_score.setFillColor(sf::Color::White);
  m_score.move(SCORE_POSITION);
  m_score.setString("SCORE: 000000");
  m_title_text.setCharacterSize(150U);
  m_title_text.setString("ASTEROIDS");
  m_title_text.setFillColor(sf::Color::White);
  m_press_enter.setCharacterSize(20U);
  m_press_enter.setString("PRESS ENTER TO START");
  m_press_enter.setFillColor(sf::Color::White);
}

DisplayManager::~DisplayManager() {
  m_game_window.close();
}

bool DisplayManager::load_resources(std::string game_font) {
  if (!m_game_font.loadFromFile(game_font)) {
    return false;
  }
  m_score.setFont(m_game_font);
  m_title_text.setFont(m_game_font);
  m_press_enter.setFont(m_game_font);
  m_level_label.setFont(m_game_font);
  return true;
}

sf::Vector2f DisplayManager::screen_center() const {
  return DISPLAY_SIZE / 2.0F;
}

sf::Vector2f DisplayManager::saucer_spawn_position() const {
  int position_index = rand() % 2;
  return SAUCER_SPAWNS.at(position_index);
}

bool DisplayManager::poll_event(sf::Event &event) {
  return m_game_window.pollEvent(event);
}

void DisplayManager::draw_screen(const StateManager &game_state, float dt,
    const std::vector<std::shared_ptr<GameObject>> &objects,
    unsigned int level) {
  std::shared_ptr<Spaceship> player = nullptr;
  float lives_offset = 20.0F;
  sf::Vector2f offset_vector{0.0F, 0.0F};
  m_game_window.clear(sf::Color::Black);
  if (game_state.game_over()) {
    m_game_window.draw(gameover_string());
  } else if (game_state.in_game() || game_state.paused()) {
    for (auto object : objects) {
      m_game_window.draw(*object->get_sprite());
      if (*object == GameObject::PlayerType) {
        player = std::dynamic_pointer_cast<Spaceship>(object);
      }
    }
    if (player) {
      for (unsigned int i = 0U; i < player->get_lives(); i++) {
        offset_vector = {(lives_offset * i), 0.0F};
        m_life_sprite.setPosition(LIFE_POSITION + offset_vector);
        m_game_window.draw(m_life_sprite);
      }
      m_score.setString("SCORE: " + std::to_string(player->get_score()));
      m_game_window.draw(m_score);
      offset_vector = {0.0F, 0.0F};
    }
    m_level_label.setString("LEVEL " + std::to_string(level));
    m_level_label.setOrigin(m_level_label.getLocalBounds().width / 2.0F, 0.0F);
    m_level_label.setPosition(LEVEL_POSITION);
    m_game_window.draw(m_level_label);
  } else if (game_state.title_screen()) {
    for (auto object : objects) {
      m_game_window.draw(*object->get_sprite());
    }
    m_title_text.setOrigin(m_title_text.getLocalBounds().width / 2.0F,
                           m_title_text.getLocalBounds().height / 2.0F);
    m_title_text.setPosition(TITLE_POSITION);
    m_game_window.draw(m_title_text);
    m_press_enter.setOrigin(m_press_enter.getLocalBounds().width / 2.0F,
                            m_press_enter.getLocalBounds().height / 2.0F);
    m_press_enter.setPosition(START_POSITION);
    m_blink_timer -= dt;
    if (m_blink_timer > 0.0F) {
      m_game_window.draw(m_press_enter);
    } else if (m_blink_timer <= -BLINK_TIMER) {
      m_blink_timer = BLINK_TIMER;
    }
  }
  m_game_window.display();
}

void DisplayManager::wrap_object(GameObject &object) {
  float wrapped_x = object.get_position().x;
  float wrapped_y = object.get_position().y;
  if (object.get_position().x <= 0.0F) {
    wrapped_x = (object.get_position().x + DISPLAY_SIZE.x +
                 (object.get_radius() * 2.0F));
  } else if (object.get_position().x >= DISPLAY_SIZE.x) {
    wrapped_x = (object.get_position().x - DISPLAY_SIZE.x -
                 (object.get_radius() * 2.0F));
  }
  if (object.get_position().y <= 0.0F) {
    wrapped_y = (object.get_position().y + DISPLAY_SIZE.y +
                 (object.get_radius() * 2.0F));
  } else if (object.get_position().y >= DISPLAY_SIZE.y) {
    wrapped_y = (object.get_position().y - DISPLAY_SIZE.y -
                 (object.get_radius() * 2.0F));
  }
  object.move_to(sf::Vector2f{wrapped_x, wrapped_y});
}

bool DisplayManager::off_camera(sf::Vector2f position, float radius) const {
  return position.x < -radius ||
         position.y < -radius ||
         position.x > DISPLAY_SIZE.x + radius ||
         position.y > DISPLAY_SIZE.y + radius;
}

sf::Vector2f DisplayManager::valid_asteroid_position(
    const std::vector<std::shared_ptr<GameObject>> &game_objects) const {
  float old_x, old_y, new_x, new_y, distance;
  bool invalid;
  do {
    invalid = false;
    new_x = rand() % static_cast<int>(DISPLAY_SIZE.x);
    new_y = rand() % static_cast<int>(DISPLAY_SIZE.y);
    if (new_x <= 50.0F || new_x >= DISPLAY_SIZE.x - 50.0F ||
        new_y <= 50.0F || new_y >= DISPLAY_SIZE.y - 50.0F) {
      invalid = true;
    }
    for (auto object : game_objects) {
      old_x = object->get_position().x;
      old_y = object->get_position().y;
      distance = sqrt(pow((old_x - new_x), 2) + pow((old_y - new_y), 2));
      if (object->get_object_type() == GameObject::PlayerType &&
          distance < DISPLAY_SIZE.y / 6.0F) {
          invalid = true;
      } else if (object->get_object_type() == GameObject::AsteroidType &&
                 distance < 110.0F) {
          invalid = true;
      }
    }
  } while (invalid);
  return sf::Vector2f{new_x, new_y};
}

sf::Text DisplayManager::gameover_string() const {
  sf::Text string{"GAME OVER", m_game_font, 100U};
  string.setFillColor(sf::Color::White);
  sf::Vector2f new_origin{string.getLocalBounds().width / 2.0F,
      string.getLocalBounds().height};
  string.setOrigin(new_origin);
  string.move(screen_center());
  return string;
}

}