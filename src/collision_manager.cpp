#include "collision_manager.hpp"

#include <cmath>
#include <memory>

#include <SFML/System.hpp>

#include "game_object.hpp"
#include "quadtree.hpp"
#include "display_manager.hpp"

namespace ag {

CollisionManager::CollisionManager(sf::Vector2f display_size)
  : m_collidables{0U, sf::FloatRect(0.0F, 0.0F, display_size.x, display_size.y)}
{}

bool CollisionManager::load_resources(std::string collision_sfx) {
  if (!m_collision_sfx_buffer.loadFromFile(collision_sfx)) {
    return false;
  }
  m_collision_sfx.setBuffer(m_collision_sfx_buffer);
  return true;
}

GameObject::ObjectType CollisionManager::collision_check(const GameObject &object,
    const std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<const GameObject *> other_objects;
  for (auto other_object : m_game_objects) {
    m_collidables.insert(*other_object);
  }
  other_objects = m_collidables.retrieve(object.get_bounds());
  bool collision = false;
  GameObject::ObjectType collider_type = GameObject::NullType;
  for (auto &&collider : other_objects) {
    if (&object != collider &&
        object.get_bounds().intersects(collider->get_bounds())) {
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
      if (m_collision_sfx.getStatus() == sf::Sound::Stopped) {
        m_collision_sfx.play();
      }
      collider_type = collider->get_object_type();
      break;
    }
  }
  m_collidables.clear();
  return collider_type;
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
    return ship_ship(ship.get_vertices(), collider.get_vertices());
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

bool CollisionManager::ship_ship(std::vector<sf::Vector2f> ship_one_vertices,
    std::vector<sf::Vector2f> ship_two_vertices) const {
  std::vector<sf::Vector2f> all_axes, ship_one_axes, ship_two_axes;
  ship_one_axes = calculate_normals(ship_one_vertices);
  ship_two_axes = calculate_normals(ship_two_vertices);
  all_axes.insert(all_axes.end(), ship_one_axes.begin(), ship_one_axes.end());
  all_axes.insert(all_axes.end(), ship_two_axes.begin(), ship_two_axes.end());
  for (auto axis : all_axes) {
    if (ships_overlap(axis, ship_one_vertices, ship_two_vertices)) {
      return false;
    }
  }
  return true;
}

bool CollisionManager::ship_circle(std::vector<sf::Vector2f> ship_vertices,
                                   sf::Vector2f circle_position,
                                   float circle_radius) const {
  std::vector<sf::Vector2f> all_axes, ship_axes;
  ship_axes = calculate_normals(ship_vertices);
  float distance = circle_radius * 2.0F;
  float minimum_distance = distance;
  sf::Vector2f circle_axis;
  for (auto vertex : ship_vertices) {
    distance = static_cast<float>(sqrt(pow((vertex.x - circle_position.x), 2) +
                                       pow((vertex.y - circle_position.y), 2)));
    if (distance < minimum_distance) {
      minimum_distance = distance;
      circle_axis = vertex - circle_position;
    }
  }
  all_axes.insert(all_axes.end(), ship_axes.begin(), ship_axes.end());
  all_axes.push_back(circle_axis);
  for (auto axis : all_axes) {
    if (ship_circle_overlap(axis, ship_vertices, circle_position,
                            circle_radius)) {
      return false;
    }
  }
  return true;
}

std::vector<sf::Vector2f> CollisionManager::calculate_normals(
    std::vector<sf::Vector2f> vertices) const {
  sf::Vector2f side;
  std::vector<sf::Vector2f> normals;
  for (unsigned int i = 0; i < vertices.size(); i++) {
    side = vertices.at((i + 1) % vertices.size()) - vertices.at(i);
    normals.push_back(sf::Vector2f{-(side.y), (side.x)});
  }
  return normals;
}

bool CollisionManager::ships_overlap(sf::Vector2f axis,
    std::vector<sf::Vector2f> ship_one_vertices,
    std::vector<sf::Vector2f> ship_two_vertices) const {
  float min_one = vector2f_dot_product(axis, ship_one_vertices.at(0));
  float max_one = min_one;
  float min_two = vector2f_dot_product(axis, ship_two_vertices.at(0));
  float max_two = min_one;
  float dot_product;
  for (auto vertex : ship_one_vertices) {
    dot_product = vector2f_dot_product(axis, vertex);
    min_one = std::min(min_one, dot_product);
    max_one = std::max(max_one, dot_product);
  }
  for (auto vertex : ship_two_vertices) {
    dot_product = vector2f_dot_product(axis, vertex);
    min_two = std::min(min_two, dot_product);
    max_two = std::max(max_two, dot_product);
  }
  return max_one < min_two || min_one > max_two;
}

bool CollisionManager::ship_circle_overlap(sf::Vector2f axis,
                           std::vector<sf::Vector2f> ship_vertices,
                           sf::Vector2f circle_position,
                           float circle_radius) const {
  float min_one = vector2f_dot_product(axis, ship_vertices.at(0));
  float max_one = min_one;
  float dot_product;
  for (auto vertex : ship_vertices) {
    dot_product = vector2f_dot_product(axis, vertex);
    min_one = std::min(min_one, dot_product);
    max_one = std::max(max_one, dot_product);
  }
  sf::Vector2f circle_vertex_one, circle_vertex_two;
  circle_vertex_one = circle_position - (normalize_vector2f(axis) * circle_radius);
  circle_vertex_two = circle_position + (normalize_vector2f(axis) * circle_radius);
  float min_two = vector2f_dot_product(axis, circle_vertex_one);
  float max_two = vector2f_dot_product(axis, circle_vertex_two);
  return max_one < min_two || min_one > max_two;
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