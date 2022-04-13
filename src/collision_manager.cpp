#include "collision_manager.h"

#include <cmath>
#include <memory>

#include <SFML/System.hpp>

#include "game_object.h"
#include "quadtree.h"
#include "helpers.h"

namespace ag {

CollisionManager::CollisionManager()
  : m_collidables{
        0U, sf::FloatRect(0.0F, 0.0F, static_cast<float>(DISPLAY_SIZE.x),
        static_cast<float>(DISPLAY_SIZE.y))} {}

void CollisionManager::check_for_collisions(const sf::Time dt,
    std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<GameObject *> other_objects;
  CollisionManager::Outcome outcome;
  for (auto object : m_game_objects) {
    m_collidables.insert(*object);
  }
  for (auto object_one : m_game_objects) {
    other_objects = m_collidables.retrieve(object_one->get_bounds());
    for (auto object_two : m_game_objects) {
      if (object_one->get_object_id() != object_two->get_object_id()){
        outcome = collision(dt, *object_one, *object_two);
        if (outcome == CollisionManager::Deflect) {
          sf::Vector2f velocity_one, velocity_two;
          velocity_one = object_one->get_velocity();
          velocity_two = object_two->get_velocity();
          sf::Vector2f n = normalize_vector2f(object_one->get_position() -
                                              object_two->get_position());
          float dot_one = velocity_one.x * n.x + velocity_one.y * n.y;
          float dot_two = velocity_two.x * n.x + velocity_two.y * n.y;
          float optimized_p = (2.0F * (dot_one - dot_two)) /
                              (object_one->get_mass() + object_two->get_mass());
          sf::Vector2f new_velocity_one = velocity_one - optimized_p *
                                          object_two->get_mass() * n;
          sf::Vector2f new_velocity_two = velocity_two + optimized_p *
                                          object_one->get_mass() * n;
          object_one->deflect(new_velocity_one);
          object_two->deflect(new_velocity_two);
        } else if (outcome == CollisionManager::Collide) {
          object_one->collide();
          object_two->collide();
        }
      }
    }
    other_objects.clear();
  }
  m_collidables.clear();
}

CollisionManager::Outcome CollisionManager::collision(const sf::Time dt,
    const GameObject &object_one, const GameObject &object_two) {
  if (object_one.get_bounds().intersects(object_two.get_bounds())) {
    if (object_one.get_object_type() == GameObject::PlayerType) {
      return player_collision(object_one.get_vertices(),
                              object_two.get_position());
    } else if (object_two.get_object_type() == GameObject::PlayerType) {
      return player_collision(object_two.get_vertices(),
                              object_one.get_position());
    } else if (object_one.get_object_type() == GameObject::AsteroidType &&
               object_two.get_object_type() == GameObject::AsteroidType) {
      sf::Vector2f future_one = object_one.get_position() +
                                object_one.get_velocity() *
                                dt.asSeconds();
      sf::Vector2f future_two = object_two.get_position() +
                                object_two.get_velocity() *
                                dt.asSeconds();
      return asteroid_collision(future_one, future_two);
    }
  }
  return CollisionManager::Miss;
}

CollisionManager::Outcome CollisionManager::player_collision(
    const std::vector<sf::Vector2f> player_vertices,
    const sf::Vector2f object_position) {
  float distance;
  for (auto vertex : player_vertices) {
    distance = sqrt(pow((vertex.x - object_position.x), 2) +
                    pow((vertex.y - object_position.y), 2));
    if (distance <= 50.0F) {
      return CollisionManager::Collide;
    }
  }
  return CollisionManager::Miss;
}

CollisionManager::Outcome CollisionManager::asteroid_collision(
    const sf::Vector2f asteroid_one, const sf::Vector2f asteroid_two) {
  float delta_x, delta_y, wrapped_delta_x, wrapped_delta_y;
  float distance, x_wrapped_distance, y_wrapped_distance, xy_wrapped_distance;
  delta_x = asteroid_one.x - asteroid_two.x;
  delta_y = asteroid_one.y - asteroid_two.y;
  distance = sqrt(pow((delta_x), 2) + pow((delta_y), 2));
  if (asteroid_one.x <= asteroid_two.x) {
    wrapped_delta_x = asteroid_one.x + DISPLAY_SIZE.x - asteroid_two.x;
  } else {
    wrapped_delta_x = asteroid_two.x + DISPLAY_SIZE.x - asteroid_one.x;
  }
  if (asteroid_one.y <= asteroid_two.y) {
    wrapped_delta_y = asteroid_one.y + DISPLAY_SIZE.y - asteroid_two.y;
  } else {
    wrapped_delta_y = asteroid_two.y + DISPLAY_SIZE.y - asteroid_one.y;
  }
  x_wrapped_distance = sqrt(pow((wrapped_delta_x), 2) + pow((delta_y), 2));
  y_wrapped_distance = sqrt(pow((delta_x), 2) + pow((wrapped_delta_y), 2));
  xy_wrapped_distance = sqrt(pow((wrapped_delta_x), 2) + pow((wrapped_delta_y),
                             2));
  if (distance <= 100.0F ||
      x_wrapped_distance <= 100.0F ||
      y_wrapped_distance <= 100.0F ||
      xy_wrapped_distance <= 100.0F) {
    return CollisionManager::Deflect;
  }
  return CollisionManager::Miss;
}

}