#include "game_object.h"

namespace ag {

const unsigned int GameObject::get_object_id() const {
  return m_object_id;
}

const GameObject::ObjectType GameObject::get_object_type() const {
  return m_object_type;
}

const bool GameObject::destroyed() const{
  return m_destroyed;
}

void GameObject::set_object_id(const unsigned int id) {
  m_object_id = id;
}

void GameObject::set_object_type(const ObjectType type) {
  m_object_type = type;
}

void GameObject::set_destroyed(const bool state) {
  m_destroyed = state;
}

}
