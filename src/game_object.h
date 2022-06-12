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

  bool operator ==(GameObject other) const;
  bool operator !=(GameObject other) const;
  unsigned int get_object_id() const;
  GameObject::ObjectType get_object_type() const;
  sf::Vector2f get_velocity() const;
  bool is_destroyed() const;
  void destroy();
  virtual const sf::Drawable *get_sprite() const {};
  virtual sf::FloatRect get_bounds() const {};
  virtual sf::Vector2f get_position() const {};
  virtual std::vector<sf::Vector2f> get_vertices() const {};
  virtual float get_radius() const {};
  virtual float get_rotation() const {};
  virtual bool is_shooting() const {};
  virtual std::shared_ptr<GameObject> spawn_bullet(unsigned int id) {};
  virtual void move_to(sf::Vector2f new_position) {};
  virtual void update(float dt) {};
  virtual std::shared_ptr<GameObject> spawn_child(float direction,
                                                  unsigned int id) {};

 protected:
  void set_object_id(unsigned int id);
  void set_object_type(ObjectType type);
  void set_velocity(sf::Vector2f velocity);
  void not_destroyed();

 private:
  unsigned int m_object_id;
  ObjectType m_object_type;
  sf::Vector2f m_velocity;
  bool m_destroyed;
};

}

#endif
