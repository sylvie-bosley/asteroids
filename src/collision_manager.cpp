#include "collision_manager.h"

#include <cmath>
#include <memory>

#include "game_object.h"
#include "quadtree.h"
#include "helpers.h"

namespace ag {

CollisionManager::CollisionManager() :
    m_collidables{0U,
                  sf::FloatRect(0.0F, 0.0F, static_cast<float>(DISPLAY_SIZE.x),
                                static_cast<float>(DISPLAY_SIZE.y))} {}

void CollisionManager::check_for_collisions(
    std::vector<std::shared_ptr<GameObject>> m_game_objects) {
  std::vector<std::shared_ptr<GameObject>> other_objects;
  for (unsigned int i = 0U; i < m_game_objects.size(); i++) {
    m_collidables.insert(m_game_objects.at(i));
  }
  for (unsigned int i = 0U; i < m_game_objects.size(); i++) {
    other_objects = m_collidables.retrieve(m_game_objects.at(i)->get_bounds());
    for (unsigned int j = 0U; j < other_objects.size(); j++) {
      if (m_game_objects.at(i)->get_object_id() !=
              other_objects.at(j)->get_object_id() &&
          collision(m_game_objects.at(i), other_objects.at(j))){
        m_game_objects.at(i)->collide();
        other_objects.at(j)->collide();
      }
    }
    other_objects.clear();
  }
  m_collidables.clear();
}

bool CollisionManager::collision(const std::shared_ptr<GameObject> object_one,
                                 const std::shared_ptr<GameObject> object_two) {
  bool collision_occurrs = false;
  if (object_one->get_bounds().intersects(object_two->get_bounds())) {
    float distance;
    std::vector<sf::Vector2f> vertices;
    if (object_one->get_object_type() == GameObject::PlayerType) {
      vertices = object_one->get_vertices();
      for (unsigned int i = 0U; i < vertices.size(); i++) {
        distance = sqrt(
            pow((vertices.at(i).x - object_two->get_position().x), 2) +
            pow((vertices.at(i).y - object_two->get_position().y), 2));
        if (distance <= 50.0F) {
          collision_occurrs = true;
        }
      }
    } else if (object_two->get_object_type() == GameObject::PlayerType) {
      vertices = object_two->get_vertices();
      for (unsigned int i = 0U; i < vertices.size(); i++) {
        distance = sqrt(
            pow((vertices.at(i).x - object_one->get_position().x), 2) +
            pow((vertices.at(i).y - object_one->get_position().y), 2));
        if (distance <= 50.0F) {
          collision_occurrs = true;
        }
      }
    }
  }
  return collision_occurrs;
}

}