#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H

#include <SFML/System.hpp>

namespace ag {

class GameObject {
 protected:
  const sf::Vector2f get_position();
  const float get_orientation();
  const sf::Vector2f get_velocity();
  void set_position(const sf::Vector2f position);
  void set_orientation(const float orientation);
  void set_velocity(const sf::Vector2f velocity);

 private:
  sf::Vector2f m_position;
  float m_orientation;
  sf::Vector2f m_velocity;
};

}

#endif
