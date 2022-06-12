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
  explicit CollisionManager(sf::Vector2f display_size);
  ~CollisionManager() {};

  bool collision_check(const GameObject &object,
    const std::vector<std::shared_ptr<GameObject>> m_game_objects);

#ifdef DEBUG
  bool load_resources(std::string collision_sfx);
#endif

 private:
  bool player_collision_checks(const GameObject &player,
                               const GameObject &collider) const;
  bool circle_collision_checks(const GameObject &circle,
                               const GameObject &collider) const;
  bool player_circle(std::vector<sf::Vector2f> player_vertices,
                     sf::Vector2f circle_position, float circle_radius) const;
  bool circle_circle(sf::Vector2f circle_one_position, float circle_one_radius,
                     sf::Vector2f circle_two_position,
                     float circle_two_radius) const;

  QuadTree m_collidables;
#ifdef DEBUG
  sf::SoundBuffer m_collision_sfx_buffer;
  sf::Sound m_collision_sfx;
#endif
};

}

#endif