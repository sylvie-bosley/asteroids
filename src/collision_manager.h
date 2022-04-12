#ifndef ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H

#include <memory>

#include <SFML/System.hpp>

#include "game_object.h"
#include "quadtree.h"

namespace ag {

class CollisionManager
{
 public:
  CollisionManager();
  ~CollisionManager() {};

  void check_for_collisions(std::vector<
      std::shared_ptr<GameObject>> m_game_objects);

 private:
  bool collision(const GameObject &object_one,
                 const GameObject &object_two);

  QuadTree m_collidables;
};

}

#endif