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
    if (&object != collider) {
      switch (object.get_object_type()) {
      case GameObject::PlayerType:
        collision = ship_collision_checks(object, *collider);
        break;
      case GameObject::AsteroidType:
        collision = circle_collision_checks(object, *collider);
        break;
      case GameObject::BulletType:
        collision = circle_collision_checks(object, *collider);
        break;
      case GameObject::SaucerType:
        collision = ship_collision_checks(object, *collider);
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

bool CollisionManager::ship_collision_checks(const GameObject &ship,
                                             const GameObject &collider)
    const {
  float x_offset = collider.get_position().x - ship.get_position().x;
  float y_offset = collider.get_position().y - ship.get_position().y;
  sf::Vector2f distance{x_offset, y_offset};
  switch (collider.get_object_type()) {
  case GameObject::PlayerType:
  case GameObject::SaucerType:
    return ship_ship(distance, ship.get_vertices(), collider.get_vertices());
  case GameObject::AsteroidType:
  case GameObject::BulletType:
    return ship_circle(ship.get_vertices(), collider.get_position(),
                       collider.get_radius());
  }
}

bool CollisionManager::circle_collision_checks(const GameObject &circle,
                                               const GameObject &collider)
      const {
  switch (collider.get_object_type()) {
  case GameObject::PlayerType:
  case GameObject::SaucerType:
    return ship_circle(collider.get_vertices(), circle.get_position(),
                       circle.get_radius());
  case GameObject::AsteroidType:
  case GameObject::BulletType:
    return circle_circle(circle.get_position(), circle.get_radius(),
                         collider.get_position(), collider.get_radius());
  }
}

bool CollisionManager::ship_ship(sf::Vector2f distance,
    std::vector<sf::Vector2f> ship_one_vertices,
    std::vector<sf::Vector2f> ship_two_vertices) const {
  std::vector<sf::Vector2f> all_axes, ship_one_axes, ship_two_axes;
  ship_one_axes = vertices_to_axes(ship_one_vertices);
  ship_two_axes = vertices_to_axes(ship_two_vertices);
  all_axes.insert(all_axes.end(), ship_one_axes.begin(), ship_one_axes.end());
  all_axes.insert(all_axes.end(), ship_two_axes.begin(), ship_two_axes.end());
  for (auto axis : all_axes) {
    if (!ships_overlap(distance, axis, ship_one_vertices, ship_two_vertices)) {
      return false;
    }
  }
  return true;
}

std::vector<sf::Vector2f> CollisionManager::vertices_to_axes(
    std::vector<sf::Vector2f> vertices) const {
  float x1, x2, y1, y2;
  std::vector<sf::Vector2f> normals;
  for (unsigned int i = 0; i < vertices.size(); i++) {
    x1 = vertices.at(i).x;
    y1 = vertices.at(i).y;
    x2 = vertices.at((i + 1) % vertices.size()).x;
    y2 = vertices.at((i + 1) % vertices.size()).y;
    normals.push_back(sf::Vector2f{-(y2 - y1), (x2 - x1)});
  }
  return normals;
}

bool CollisionManager::ships_overlap(sf::Vector2f axis, sf::Vector2f distance,
    std::vector<sf::Vector2f> ship_one_vertices,
    std::vector<sf::Vector2f> ship_two_vertices) const {
  float min_one = vector2f_dot_product(ship_one_vertices.at(0), axis);
  float max_one = min_one;
  float min_two = vector2f_dot_product(ship_two_vertices.at(0), axis);
  float max_two = min_one;
  float dot_product;
  for (auto vertex : ship_one_vertices) {
    dot_product = vector2f_dot_product(vertex, axis);
    min_one = std::min(min_one, dot_product);
    max_one = std::max(max_one, dot_product);
  }
  for (auto vertex : ship_two_vertices) {
    dot_product = vector2f_dot_product(vertex, axis);
    min_two = std::min(min_two, dot_product);
    max_two = std::max(max_two, dot_product);
  }
  return (min_two < max_one && min_two > min_one) ||
         (max_two > min_one && max_two < max_one);
}

bool CollisionManager::ship_circle(std::vector<sf::Vector2f> ship_vertices,
                                   sf::Vector2f circle_position,
                                   float circle_radius) const {
  float distance;
  for (auto vertex : ship_vertices) {
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