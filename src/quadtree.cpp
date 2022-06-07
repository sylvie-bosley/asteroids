#include "quadtree.h"

#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "helpers.h"

namespace ag {

QuadTree::QuadTree(unsigned int level, sf::FloatRect world_area) {
   m_level = level;
   m_bounds = world_area;
}

void QuadTree::clear() {
  m_collidables.clear();
  for (auto&& node : m_nodes) {
    node.clear();
  }
}

void QuadTree::insert(const GameObject &object) {
  if (!m_nodes.empty()) {
    int index = get_index(object.get_bounds());
    if (index != -1) {
      m_nodes.at(index).insert(object);
      return;
    }
  }
  m_collidables.push_back(&object);
  if (m_collidables.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
    if (m_nodes.empty()) {
        split();
    }
    unsigned int i = 0U;
    while (i < m_collidables.size()) {
      int index = get_index(m_collidables.at(i)->get_bounds());
      if (index != -1) {
        m_nodes.at(index).insert(*m_collidables.at(i));
        m_collidables.erase(m_collidables.begin() + i);
      }
      else {
        i++;
      }
    }
  }
}

std::vector<const GameObject *> QuadTree::retrieve(sf::FloatRect object_bounds)
    const {
  std::vector<const GameObject *> other_objects;
  int index = get_index(object_bounds);
  if (index != -1 && !m_nodes.empty()) {
    other_objects = m_nodes.at(index).retrieve(object_bounds);
  }
  for (auto object : m_collidables) {
    other_objects.push_back(object);
  }
  return other_objects;
}

void QuadTree::split() {
  float sub_width = (m_bounds.width / 2.0F);
  float sub_height = (m_bounds.height / 2.0F);
  float sub_x = m_bounds.left;
  float sub_y = m_bounds.top;
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x, sub_y, sub_width,
                                           sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x + sub_width, sub_y, sub_width,
                                           sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x, sub_y + sub_height, sub_width,
                                           sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x + sub_width,
                                           sub_y + sub_height, sub_width,
                                           sub_height}));
}

int QuadTree::get_index(sf::FloatRect bound_box) const {
  int index = -1;
  float vertical_midpoint = m_bounds.left + (m_bounds.width / 2.0F);
  float horizontal_midpoint = m_bounds.top + (m_bounds.height / 2.0F);
  bool top_half = (bound_box.top < horizontal_midpoint &&
                   bound_box.top + bound_box.height < horizontal_midpoint);
  bool bottom_half = (bound_box.top > horizontal_midpoint);
  if (bound_box.left < vertical_midpoint &&
      bound_box.left + bound_box.width < vertical_midpoint) {
    if (top_half) {
      index = 0;
    } else if (bottom_half) {
      index = 2;
    }
  } else if (bound_box.left > vertical_midpoint) {
    if (top_half) {
      index = 1;
    } else if (bottom_half) {
      index = 3;
    }
  }
  return index;
}

}
