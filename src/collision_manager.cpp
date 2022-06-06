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
bool CollisionManager::load_resources(const std::string collision_sfx) {
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

bool CollisionManager::player_collision_checks(GameObject &player,
                                               GameObject &collider) {
  switch (collider.get_object_type()) {
  case GameObject::AsteroidType:
    return player_asteroid(player.get_vertices(), collider);
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

bool CollisionManager::asteroid_collision_checks(GameObject &asteroid,
                                                 GameObject &collider) {
  switch (collider.get_object_type()) {
  case GameObject::AsteroidType:
    return asteroid_asteroid(asteroid, collider);
  case GameObject::BulletType:
    // TODO: Asteroid-Bullet collision
  case GameObject::SaucerType:
    // TODO: Asteroid-Saucer collision
  default:
    return false;
  }
}

bool CollisionManager::player_asteroid(
    const std::vector<sf::Vector2f> player_vertices, GameObject &asteroid) {
  float distance;
  for (auto vertex : player_vertices) {
    distance = sqrt(pow((vertex.x - asteroid.get_position().x), 2) +
                    pow((vertex.y - asteroid.get_position().y), 2));
    if (distance <= asteroid.get_radius()) {
      return true;
    }
  }
  return false;
}

bool CollisionManager::asteroid_asteroid(GameObject &asteroid_one,
                                         GameObject &asteroid_two) {
  float delta_x, delta_y, wrapped_delta_x, wrapped_delta_y;
  float distance, x_wrapped_distance, y_wrapped_distance, xy_wrapped_distance;
#ifdef DEBUG
  sf::Vector2f debug_pos_one = asteroid_one.get_position();
  sf::Vector2f debug_pos_two = asteroid_two.get_position();
  float debug_rad_one = asteroid_one.get_radius();
  float debug_rad_two = asteroid_two.get_radius();
#endif
  delta_x = asteroid_one.get_position().x - asteroid_two.get_position().x;
  delta_y = asteroid_one.get_position().y - asteroid_two.get_position().y;
  distance = sqrt(pow((delta_x), 2) + pow((delta_y), 2));
  if (distance <= asteroid_one.get_radius() + asteroid_two.get_radius()) {
    return true;
  }
  return false;
}

}