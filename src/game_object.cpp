#include "game_object.h"

namespace ag {

const bool GameObject::operator ==(const GameObject other) const {
  return other.get_object_id() == get_object_id();
}

const bool GameObject::operator !=(const GameObject other) const {
  return other.get_object_id() != get_object_id();
}

const unsigned int GameObject::get_object_id() const {
  return m_object_id;
}

const GameObject::ObjectType GameObject::get_object_type() const {
  return m_object_type;
}

const sf::Vector2f GameObject::get_velocity() const {
  return m_velocity;
}

const bool GameObject::is_destroyed() const{
  return m_destroyed;
}

void GameObject::set_object_id(const unsigned int id) {
  m_object_id = id;
}

void GameObject::set_object_type(const ObjectType type) {
  m_object_type = type;
}

void GameObject::set_velocity(const sf::Vector2f velocity) {
  m_velocity = velocity;
}

void GameObject::set_destroyed(const bool state) {
  m_destroyed = state;
}

}
