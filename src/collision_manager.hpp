#ifndef ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H
#define ASTEROIDS_GAME_CODE_INCLUDE_COLLISION_MANAGER_H

#include <memory>

#include <SFML/Audio.hpp>

#include "game_object.hpp"
#include "quadtree.hpp"

namespace ag {

class CollisionManager {
 public:
  CollisionManager() {};
  explicit CollisionManager(sf::Vector2f display_size);
  ~CollisionManager() {};

  GameObject::ObjectType collision_check(const GameObject &object,
    const std::vector<std::shared_ptr<GameObject>> m_game_objects);

  bool load_resources(std::string collision_sfx);

 private:
  bool ship_collision_checks(const GameObject &ship,
                             const GameObject &collidable) const;
  bool circle_collision_checks(const GameObject &circle,
                               const GameObject &collidable) const;
  bool ship_ship(std::vector<sf::Vector2f> ship_one_vertices,
                 std::vector<sf::Vector2f> ship_two_vertices) const;
  bool ship_circle(std::vector<sf::Vector2f> ship_vertices,
                   sf::Vector2f circle_position, float circle_radius) const;
  std::vector<sf::Vector2f> calculate_normals(
    std::vector<sf::Vector2f> vertices) const;
  bool ships_overlap(sf::Vector2f axis,
                     std::vector<sf::Vector2f> ship_one_vertices,
                     std::vector<sf::Vector2f> ship_two_vertices) const;
  bool ship_circle_overlap(sf::Vector2f axis,
                           std::vector<sf::Vector2f> ship_vertices,
                           sf::Vector2f circle_position,
                           float circle_radius) const;
  bool circle_circle(sf::Vector2f circle_one_position, float circle_one_radius,
                     sf::Vector2f circle_two_position,
                     float circle_two_radius) const;

  QuadTree m_collidables;
  sf::SoundBuffer m_collision_sfx_buffer;
  sf::Sound m_collision_sfx;
};

}

#endif