#include "collision_manager.h"

#include <cmath>
#include <memory>

#include <SFML/System.hpp>

#include "game_object.h"
#include "quadtree.h"
#include "display_manager.h"

namespace ag {

CollisionManager::CollisionManager(sf::Vector2f display_size)
  : m_collidables{0U, sf::FloatRect(0.0F, 0.0F, display_size.x, display_size.y)}
{}

#ifdef DEBUG
bool CollisionManager::load_resources(std::string collision_sfx) {
  bool loaded = true;
  if (!m_collision_sfx_buffer.loadFromFile(collision_sfx)) {
    loaded - false;
  }
  m_collision_sfx.setBuffer(m_collision_sfx_buffer);
  return loaded;
}
#endif

void CollisionManager::collision_check(GameObject &object_one,
    std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<GameObject *> other_objects;
  for (auto object : m_game_objects) {
    m_collidables.insert(*object);
  }
  other_objects = m_collidables.retrieve(object_one.get_bounds());
  bool collision = false;
  for (auto object_two : other_objects) {
    if (object_one != *object_two &&
        object_one.get_bounds().intersects(object_two->get_bounds())) {
      switch (object_one.get_object_type()) {
      case GameObject::PlayerType:
        collision = player_collision_checks(object_one, *object_two);
        break;
      case GameObject::AsteroidType:
        collision = asteroid_collision_checks(object_one, *object_two);
        break;
      case GameObject::BulletType:
        // TODO: Bullet collision checks
        break;
      default:
        break;
      }
    }
    if (collision) {
#ifdef DEBUG
      if (m_collision_sfx.getStatus() == sf::Sound::Stopped) {
        m_collision_sfx.play();
      }
#endif
      object_one.collide();
      object_two->collide();
      break;
    }
  }
  m_collidables.clear();
}

bool CollisionManager::player_collision_checks(const GameObject &player,
                                               const GameObject &collider)
    const {
  switch (collider.get_object_type()) {
  case GameObject::AsteroidType:
    return player_asteroid(player.get_vertices(), collider.get_position(),
                           collider.get_radius());
    break;
  case GameObject::BulletType:
    // TODO: Player-Bullet collision
    break;
  case GameObject::SaucerType:
    // TODO: Player-Saucer collision
    break;
  default:
    break;
  }
}

bool CollisionManager::asteroid_collision_checks(const GameObject &asteroid,
                                                 const GameObject &collider)
      const {
  switch (collider.get_object_type()) {
  case GameObject::AsteroidType:
    return asteroid_asteroid(asteroid.get_position(), asteroid.get_radius(),
                             collider.get_position(), collider.get_radius());
  case GameObject::BulletType:
    // TODO: Asteroid-Bullet collision
  case GameObject::SaucerType:
    // TODO: Asteroid-Saucer collision
  default:
    return false;
  }
}

bool CollisionManager::player_asteroid(std::vector<sf::Vector2f> player_vertices,
                                       sf::Vector2f asteroid_position,
                                       float asteroid_radius)
    const {
  float distance;
  for (auto vertex : player_vertices) {
    distance = sqrt(pow((vertex.x - asteroid_position.x), 2) +
                    pow((vertex.y - asteroid_position.y), 2));
    if (distance <= asteroid_radius) {
      return true;
    }
  }
  return false;
}

bool CollisionManager::asteroid_asteroid(sf::Vector2f asteroid_one_position,
                                         float asteroid_one_radius,
                                         sf::Vector2f asteroid_two_position,
                                         float asteroid_two_radius) const {
  float delta_x, delta_y, distance;
  delta_x = asteroid_one_position.x - asteroid_two_position.x;
  delta_y = asteroid_one_position.y - asteroid_two_position.y;
  distance = sqrt(pow((delta_x), 2) + pow((delta_y), 2));
  if (distance <= asteroid_one_radius + asteroid_two_radius) {
    return true;
  }
  return false;
}

}