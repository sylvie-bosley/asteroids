#ifndef ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H
#define ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H

#include <vector>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "helpers.h"

namespace ag {

class QuadTree {
 public:
  QuadTree() {};
  explicit QuadTree(unsigned int new_level, sf::IntRect new_bounds);
  ~QuadTree() {};

  void clear();
  void split();
  int get_index(const sf::IntRect game_object);

 private:

  const unsigned int MAX_OBJECTS = 10;
  const unsigned int MAX_LEVELS = 5;

  unsigned int m_level;
  std::vector<GameObject> m_objects;
  sf::IntRect m_bounds;
  std::vector<QuadTree> m_nodes;

};

}

#endif
