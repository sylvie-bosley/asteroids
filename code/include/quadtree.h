#ifndef ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H
#define ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H

#include "game_object.h"

#include <vector>
#include <SFML/Graphics.hpp>

namespace ag {

class Quadtree {
 public:
  Quadtree() {};
  ~Quadtree() {};

 private:
  const unsigned int MAX_OBJECTS = 10;
  const unsigned int MAX_LEVELS = 5;
 
  unsigned int m_level;
  std::vector<GameObject> m_objects;
  //sf:: bounds;
  std::vector<Quadtree> m_nodes;

};

}

#endif
