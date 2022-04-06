#include "quadtree.h"

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "helpers.h"

namespace ag {

QuadTree::QuadTree(const unsigned int level, const sf::FloatRect world_area) {
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
  float sub_width = (m_bounds.width / 2.0F);
  float sub_height = (m_bounds.height / 2.0F);
  float sub_x = m_bounds.left;
  float sub_y = m_bounds.top;

  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x, sub_y, sub_width, sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x + sub_width, sub_y, sub_width,
                                         sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x, sub_y + sub_height, sub_width,
                                         sub_height}));
  m_nodes.push_back(QuadTree(m_level + 1U,
                             sf::FloatRect{sub_x + sub_width, sub_y + sub_height,
                                         sub_width, sub_height}));
}

int QuadTree::get_index(const sf::FloatRect bound_box) {
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

void QuadTree::insert(GameObject &collidable) {
   if (!m_nodes.empty()) {
     int index = get_index(collidable.get_sprite_bounds());

     if (index != -1) {
       m_nodes[index].insert(collidable);

       return;
     }
   }

   m_objects.push_back(collidable);

   if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
      if (m_nodes.empty()) {
         split();
      }

     unsigned int i = 0;
     while (i < m_objects.size()) {
       int index = get_index(m_objects.at(i).get_sprite_bounds());
       if (index != -1) {
         m_nodes.at(index).insert(m_objects.at(i));
         m_objects.erase(m_objects.begin() + i);
       }
       else {
         i++;
       }
     }
   }
 }

}
