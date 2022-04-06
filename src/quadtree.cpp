#include "quadtree.h"

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "helpers.h"

namespace ag {

QuadTree::QuadTree(const unsigned int level, const sf::IntRect world_area) {
   m_level = level;
   m_bounds = world_area;
}

void QuadTree::clear() {
  m_objects.clear();
  for (int i = 0; i < m_nodes.size(); i++) {
    if (!m_nodes.empty()) {
      m_nodes[i].clear();
    }
  }
}

void QuadTree::split() {
  int sub_width = static_cast<int>(m_bounds.width / 2.0F);
  int sub_height = static_cast<int>(m_bounds.height / 2.0F);
  int sub_x = m_bounds.left;
  int sub_y = m_bounds.top;

  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::IntRect{sub_x, sub_y, sub_width, sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::IntRect{sub_x + sub_width, sub_y, sub_width,
                                         sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::IntRect{sub_x, sub_y + sub_height, sub_width,
                                         sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::IntRect{sub_x + sub_width, sub_y + sub_height,
                                         sub_width, sub_height}));
}

int QuadTree::get_index(const sf::IntRect bound_box) {
  int index = -1;
  float vertical_midpoint = m_bounds.left + (m_bounds.width / 2.0F);
  float horizontal_midpoint = m_bounds.top + (m_bounds.height / 2.0F);
  bool top_quadrant = (bound_box.top < horizontal_midpoint &&
                       bound_box.top + bound_box.height < horizontal_midpoint);
  bool bottom_quadrant = (bound_box.top > horizontal_midpoint);
  if (bound_box.left < vertical_midpoint &&
      bound_box.left + bound_box.width < vertical_midpoint) {
    if (top_quadrant) {
      index = 1;
    } else if (bottom_quadrant) {
      index = 2;
    }
  } else if (bound_box.left > vertical_midpoint) {
    if (top_quadrant) {
      index = 0;
    } else if (bottom_quadrant) {
      index = 3;
    }
  }
  return index;
}

void insert(GameObject pRect) {
   if (nodes[0] != null) {
     int index = getIndex(pRect);
 
     if (index != -1) {
       nodes[index].insert(pRect);
 
       return;
     }
   }
 
   objects.add(pRect);
 
   if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
      if (nodes[0] == null) { 
         split(); 
      }
 
     int i = 0;
     while (i < objects.size()) {
       int index = getIndex(objects.get(i));
       if (index != -1) {
         nodes[index].insert(objects.remove(i));
       }
       else {
         i++;
       }
     }
   }
 }

}
