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

void CollisionManager::check_for_collisions(GameObject &object_one,
    std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<GameObject *> other_objects;
  CollisionManager::Outcome outcome;
  for (auto object : m_game_objects) {
    m_collidables.insert(*object);
  }
  other_objects = m_collidables.retrieve(object_one.get_bounds());
  for (auto object_two : other_objects) {
    if (object_one.get_object_id() != object_two->get_object_id()){
      outcome = collision_outcome(object_one, *object_two);
      if (outcome == CollisionManager::Deflect) {
        sf::Vector2f velocity_one, new_velocity_one, n,
                      velocity_two, new_velocity_two;
        float dot_one, dot_two, optimized_p;
        velocity_one = object_one.get_velocity();
        velocity_two = object_two->get_velocity();
        n = normalize_vector2f(object_one.get_position() -
                                object_two->get_position());
        dot_one = velocity_one.x * n.x + velocity_one.y * n.y;
        dot_two = velocity_two.x * n.x + velocity_two.y * n.y;
        optimized_p = (2.0F * (dot_one - dot_two)) /
                      (object_one.get_mass() + object_two->get_mass());
        new_velocity_one = velocity_one - optimized_p *
                            object_two->get_mass() * n;
        new_velocity_two = velocity_two + optimized_p *
                            object_one.get_mass() * n;
        object_one.deflect(new_velocity_one);
        object_two->deflect(new_velocity_two);
      } else if (outcome == CollisionManager::Collide) {
        object_one.collide();
        object_two->collide();
      }
    }
  }
  m_collidables.clear();
}

CollisionManager::Outcome CollisionManager::collision_outcome(
    const GameObject &object_one, const GameObject &object_two) {
  if (object_one.get_bounds().intersects(object_two.get_bounds()) ||
      (m_collidables.get_index(object_one.get_bounds()) == -1 &&
       m_collidables.get_index(object_two.get_bounds()) == -1)) {
    if (object_one.get_object_type() == GameObject::PlayerType) {
      return player_collision(object_one.get_vertices(),
                              object_two.get_position());
    } else if (object_two.get_object_type() == GameObject::PlayerType) {
      return player_collision(object_two.get_vertices(),
                              object_one.get_position());
    } else if (object_one.get_object_type() == GameObject::AsteroidType &&
               object_two.get_object_type() == GameObject::AsteroidType) {
      return asteroid_collision(object_one.get_position(),
                                object_two.get_position());
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
  if (distance <= 100.0F) {
    return CollisionManager::Deflect;
  }
  return CollisionManager::Miss;
}

}