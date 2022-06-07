#ifndef ASTEROIDS_GAME_CODE_INCLUDE_BULLET_H
#define ASTEROIDS_GAME_CODE_INCLUDE_BULLET_H

#include "game_object.h"

namespace ag {

class Bullet : public GameObject {
 public:
  Bullet() {};
  ~Bullet() {};

 private:


  float m_ttl;
  sf::CircleShape m_sprite;
};

}

#endif
