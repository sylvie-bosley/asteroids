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
  explicit QuadTree(const unsigned int level, const sf::FloatRect world_area);
  ~QuadTree() {};

  void clear();
  void split();
  int get_index(const sf::FloatRect bound_box);
  void insert(GameObject &collidable);

 private:

  const unsigned int MAX_OBJECTS = 10;
  const unsigned int MAX_LEVELS = 5;

  unsigned int m_level;
  std::vector<GameObject> m_objects;
  sf::FloatRect m_bounds;
  std::vector<QuadTree> m_nodes;

};

}

#endif
