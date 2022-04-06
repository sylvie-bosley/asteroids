#include "quadtree.h"

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "helpers.h"

namespace ag {

QuadTree::QuadTree(unsigned int new_level, sf::IntRect new_bounds) {
   m_level = new_level;
   m_bounds = new_bounds;
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

int QuadTree::get_index(const sf::IntRect rect) {
  int index = -1;
  float vertical_midpoint = m_bounds.left + (m_bounds.width / 2.0F);
  float horizontal_midpoint = m_bounds.top + (m_bounds.height / 2.0F);
  bool top_quadrant = (rect.top < horizontal_midpoint &&
                       rect.top + rect.height < horizontal_midpoint);
  bool bottom_quadrant = (rect.top > horizontal_midpoint);
  if (rect.left < vertical_midpoint &&
      rect.left + rect.width < vertical_midpoint) {
    if (top_quadrant) {
      index = 1;
    } else if (bottom_quadrant) {
      index = 2;
    }
  } else if (rect.left > vertical_midpoint) {
    if (top_quadrant) {
      index = 0;
    } else if (bottom_quadrant) {
      index = 3;
    }
  }
  return index;
}

}
