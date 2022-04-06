#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace ag {

class GameObject {
 public:
  const unsigned int get_object_id() const;
  virtual sf::FloatRect get_sprite_bounds() {};

 protected:
  void set_object_id(const unsigned int new_id);

 private:
  unsigned int m_object_id;
};

}

#endif
