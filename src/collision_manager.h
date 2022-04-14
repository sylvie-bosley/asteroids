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

  void check_for_collisions(
      std::vector<std::shared_ptr<GameObject>> m_game_objects);

 private:
  enum Outcome {
    Miss = 0U,
    Collide = 1U,
    Deflect = 2U
  };

  CollisionManager::Outcome collision_check(const GameObject &object_one,
                                            const GameObject &object_two);
  CollisionManager::Outcome player_collision(
      const std::vector<sf::Vector2f> player_vertices,
      const sf::Vector2f object_position);
  CollisionManager::Outcome asteroid_collision(const sf::Vector2f asteroid_one,
                                               const sf::Vector2f asteroid_two);

  QuadTree m_collidables;
};

}

#endif