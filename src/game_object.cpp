#include "game_object.h"

namespace ag {

bool GameObject::operator ==(GameObject &other) const {
  return m_object_id == other.get_object_id();
}

bool GameObject::operator ==(GameObject::ObjectType type) const {
  return m_object_type == type;
}

bool GameObject::operator !=(GameObject &other) const {
  return m_object_id != other.get_object_id();
}

bool GameObject::operator !=(GameObject::ObjectType type) const {
  return m_object_type != type;
}

GameObject::ObjectType GameObject::get_object_type() const {
  return m_object_type;
}

sf::Vector2f GameObject::get_velocity() const {
  return m_velocity;
}

bool GameObject::is_destroyed() const{
  return m_destroyed;
}

void GameObject::destroy() {
  m_destroyed = true;
}

unsigned int GameObject::get_object_id() const {
  return m_object_id;
}

void GameObject::set_object_id(unsigned int id) {
  m_object_id = id;
}

void GameObject::set_object_type(ObjectType type) {
  m_object_type = type;
}

void GameObject::set_velocity(sf::Vector2f velocity) {
  m_velocity = velocity;
}

void GameObject::not_destroyed() {
  m_destroyed = false;
}

}
