#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H

#include <SFML/Graphics.hpp>

namespace ag {

class GameObject {
 public:
  enum ObjectType {
    PlayerType,
    AsteroidType
  };

  const unsigned int get_object_id() const;
  const GameObject::ObjectType get_object_type() const;
  const sf::Vector2f get_velocity() const;
  const bool is_destroyed() const;
  virtual const sf::Drawable *get_sprite() const {};
  virtual const sf::FloatRect get_bounds() const {};
  virtual const sf::Vector2f get_position() const {};
  virtual const std::vector<sf::Vector2f> get_vertices() const {};
  virtual const float get_mass() const {};
  virtual void update(const sf::Time dt) {};
  virtual void deflect(const sf::Vector2f new_velocity) {};
  virtual void clear_deflect() {};
  virtual void collide() {};

 protected:
  void set_object_id(const unsigned int id);
  void set_object_type(const ObjectType type);
  void set_velocity(const sf::Vector2f velocity);
  void set_destroyed(const bool state);

 private:
  unsigned int m_object_id;
  ObjectType m_object_type;
  sf::Vector2f m_velocity;
  bool m_destroyed;
};

}

#endif
