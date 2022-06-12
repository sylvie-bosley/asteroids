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

bool CollisionManager::collision_check(const GameObject &object,
    const std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<const GameObject *> other_objects;
  for (auto other_object : m_game_objects) {
    m_collidables.insert(*other_object);
  }
  other_objects = m_collidables.retrieve(object.get_bounds());
  bool collision = false;
  for (auto &&collider : other_objects) {
    if (object != *collider) {
      switch (object.get_object_type()) {
      case GameObject::PlayerType:
        collision = player_collision_checks(object, *collider);
        break;
      case GameObject::AsteroidType:
        collision = circle_collision_checks(object, *collider);
        break;
      case GameObject::BulletType:
        collision = circle_collision_checks(object, *collider);
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
      break;

    }
  }
  m_collidables.clear();
  return collision;
}

bool CollisionManager::player_collision_checks(const GameObject &player,
                                               const GameObject &collider)
    const {
  switch (collider.get_object_type()) {
  case GameObject::AsteroidType:
    return player_circle(player.get_vertices(), collider.get_position(),
                         collider.get_radius());
  case GameObject::BulletType:
    return player_circle(player.get_vertices(), collider.get_position(),
                         collider.get_radius());
  case GameObject::SaucerType:
    // TODO: Player-Saucer collision
  default:
    return false;
  }
}

bool CollisionManager::circle_collision_checks(const GameObject &circle,
                                               const GameObject &collider)
      const {
  switch (collider.get_object_type()) {
  case GameObject::PlayerType:
    return player_circle(collider.get_vertices(), circle.get_position(),
                         circle.get_radius());
  case GameObject::AsteroidType:
    return circle_circle(circle.get_position(), circle.get_radius(),
                         collider.get_position(), collider.get_radius());
  case GameObject::BulletType:
    return circle_circle(circle.get_position(), circle.get_radius(),
                         collider.get_position(), collider.get_radius());
  case GameObject::SaucerType:
    // TODO: Asteroid-Saucer collision
  default:
    return false;
  }
}

bool CollisionManager::player_circle(std::vector<sf::Vector2f> player_vertices,
                                     sf::Vector2f circle_position,
                                     float circle_radius)
    const {
  float distance;
  for (auto vertex : player_vertices) {
    distance = sqrt(pow((vertex.x - circle_position.x), 2) +
                    pow((vertex.y - circle_position.y), 2));
    if (distance <= circle_radius) {
      return true;
    }
  }
  return false;
}

bool CollisionManager::circle_circle(sf::Vector2f circle_one_position,
                                     float circle_one_radius,
                                     sf::Vector2f circle_two_position,
                                     float circle_two_radius) const {
  float delta_x, delta_y, distance;
  delta_x = circle_one_position.x - circle_two_position.x;
  delta_y = circle_one_position.y - circle_two_position.y;
  distance = sqrt(pow((delta_x), 2) + pow((delta_y), 2));
  return distance <= circle_one_radius + circle_two_radius;
}

}