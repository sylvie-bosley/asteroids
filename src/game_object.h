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

  bool operator ==(GameObject &other) const;
  bool operator ==(GameObject::ObjectType type) const;
  bool operator !=(GameObject &other) const;
  bool operator !=(GameObject::ObjectType type) const;
  GameObject::ObjectType get_object_type() const;
  sf::Vector2f get_velocity() const;
  bool is_destroyed() const;
  virtual const sf::Drawable *get_sprite() const=0;
  virtual sf::FloatRect get_bounds() const=0;
  virtual sf::Vector2f get_position() const=0;
  virtual std::vector<sf::Vector2f> get_vertices() const {
    return std::vector<sf::Vector2f>{}; };
  virtual float get_radius() const=0;
  virtual bool is_shooting() const { return false; };
  virtual void collide()=0;
  virtual void move_to(sf::Vector2f new_position)=0;
  virtual void update(float dt)=0;
  virtual void aim(sf::Vector2f player_position) {};
  virtual std::shared_ptr<GameObject> spawn_child(unsigned int id,
    float direction = 0.0F) { return nullptr; };

 protected:
  unsigned int get_object_id() const;
  void set_object_id(unsigned int id);
  void set_object_type(ObjectType type);
  void set_velocity(sf::Vector2f velocity);
  void set_destroyed(bool new_state);

 private:
  unsigned int m_object_id;
  ObjectType m_object_type;
  sf::Vector2f m_velocity;
  bool m_destroyed;
};

}

#endif
