#include "include/game_object.h"

namespace ag {

const sf::Vector2f GameObject::get_position() {
  return m_position;
}

const float GameObject::get_orientation() {
  return m_orientation;
}

const sf::Vector2f GameObject::get_velocity() {
  return m_velocity;
}

void GameObject::set_position(const sf::Vector2f position) {
  m_position = position;
}

void GameObject::set_orientation(const float orientation) {
  m_orientation = orientation;
}

void GameObject::set_velocity(const sf::Vector2f velocity) {
  m_velocity = velocity;
}

}
