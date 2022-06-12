#include "display_manager.h"

#include <cmath>

#include <SFML/Graphics.hpp>

namespace ag{

DisplayManager::DisplayManager ()
  : m_game_window{sf::VideoMode(static_cast<unsigned int>(DISPLAY_SIZE.x),
                                static_cast<unsigned int>(DISPLAY_SIZE.y)),
                  "Asteroids"} {}

DisplayManager::~DisplayManager() {
  m_game_window.close();
}

sf::Vector2f DisplayManager::screen_center() const {
  return DISPLAY_SIZE / 2.0F;
}

bool DisplayManager::poll_event(sf::Event &event) {
  return m_game_window.pollEvent(event);
}

void DisplayManager::clear_screen() {
  m_game_window.clear(sf::Color::Black);
}

void DisplayManager::draw(const sf::Drawable *object) {
  m_game_window.draw(*object);
}

void DisplayManager::render() {
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

}