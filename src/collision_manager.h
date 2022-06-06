#ifndef ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H

#include <memory>

#include <SFML/Audio.hpp>

#include "game_object.h"
#include "quadtree.h"

namespace ag {

class CollisionManager {
 public:
  CollisionManager() {};
  explicit CollisionManager(const sf::Vector2f display_size);
  ~CollisionManager() {};

  void collision_check(GameObject &object_one,
      std::vector<std::shared_ptr<GameObject>> m_game_objects);

#ifdef DEBUG
  bool load_resources(const std::string collision_sfx);
#endif

 private:
  bool player_collision_checks(GameObject &player, GameObject &collider);
  bool asteroid_collision_checks(GameObject &asteroid, GameObject &collider);
  bool player_asteroid(const std::vector<sf::Vector2f> player_vertices,
                       GameObject &asteroid);
  bool asteroid_asteroid(GameObject &asteroid_one, GameObject &asteroid_two);

  QuadTree m_collidables;
#ifdef DEBUG
  sf::SoundBuffer m_collision_sfx_buffer;
  sf::Sound m_collision_sfx;
#endif
};

}

#endif