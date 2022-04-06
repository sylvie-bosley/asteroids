#include "game_object.h"

namespace ag {

const unsigned int GameObject::get_object_id() const {
  return m_object_id;
}

void GameObject::set_object_id(const unsigned int id) {
  m_object_id = id;
}

}
