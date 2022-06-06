#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_OBJECT_H

#include <memory>

#include <SFML/Graphics.hpp>

namespace ag {

class GameObject {
 public:
  enum ObjectType {
    PlayerType,
    AsteroidType,
    BulletType,
    SaucerType
  };

  const bool operator ==(const GameObject other) const;
  const bool operator !=(const GameObject other) const;
  const unsigned int get_object_id() const;
  const GameObject::ObjectType get_object_type() const;
  const sf::Vector2f get_velocity() const;
  const bool is_destroyed() const;
  virtual const sf::Drawable *get_sprite() const {};
  virtual const sf::FloatRect get_bounds() const {};
  virtual const sf::Vector2f get_position() const {};
  virtual const std::vector<sf::Vector2f> get_vertices() const {};
  virtual const float get_radius() const {};
  virtual const float get_rotation() const {};
  virtual void move_to(sf::Vector2f new_position) {};
  virtual void collide() {};
  virtual void update(const float dt) {};
  virtual std::shared_ptr<GameObject> spawn_child(const GameObject &parent,
                                                  const float direction,
                                                  const unsigned int id) {};

 protected:
  void set_object_id(const unsigned int id);
  void set_object_type(const ObjectType type);
  void set_velocity(const sf::Vector2f velocity);
  void set_destroyed(const bool state);
  virtual void set_radius(const float radius) {};

 private:
  unsigned int m_object_id;
  ObjectType m_object_type;
  sf::Vector2f m_velocity;
  bool m_destroyed;
};

}

#endif
