#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace ag {

class GameObject {
 public:
  enum ObjectType {
    PlayerType,
    AsteroidType
  };

  const unsigned int get_object_id() const;
  const unsigned int get_object_type() const;
  virtual sf::FloatRect get_sprite_bounds() {};

 protected:
  void set_object_id(const unsigned int id);
  void set_object_type(const ObjectType type);

 private:
  unsigned int m_object_id;
  ObjectType m_object_type;
};

}

#endif
