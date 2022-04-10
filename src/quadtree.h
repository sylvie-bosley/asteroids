#ifndef ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H
#define ASTEROIDS_GAME_CODE_INCLUDE_QUADTREE_H

#include <memory>

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
  void insert(std::shared_ptr<GameObject> object);
  std::vector<std::shared_ptr<GameObject>> retrieve(
      sf::FloatRect object_bounds);

 private:
  const unsigned int MAX_OBJECTS = 10;
  const unsigned int MAX_LEVELS = 5;

  void split();
  int get_index(const sf::FloatRect bound_box);

  unsigned int m_level;
  std::vector<std::shared_ptr<GameObject>> m_collidables;
  sf::FloatRect m_bounds;
  std::vector<QuadTree> m_nodes;
};

}

#endif
