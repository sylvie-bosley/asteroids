#ifndef ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H

#include <memory>

#include "game_object.h"
#include "quadtree.h"

namespace ag {

class CollisionManager
{
 public:
  CollisionManager();
  ~CollisionManager() {};

  void check_for_collisions(
      std::vector<std::shared_ptr<GameObject>> m_game_objects);

 private:
  bool collision(const std::shared_ptr<GameObject> object_one,
                 const std::shared_ptr<GameObject> object_two);

  QuadTree m_collidables;
};

}

#endif