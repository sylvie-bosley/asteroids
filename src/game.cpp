#include "game.h"

#include <string>
#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"
#include "saucer.h"
#include "collision_manager.h"
#include "display_manager.h"
#include "state_manager.h"

namespace ag {

Game::Game()
    : m_difficulty{0U}, m_next_object_id{0U}, m_saucer_timer{SAUCER_INTERVAL} {
  m_player = std::make_shared<Spaceship>(m_next_object_id++,
                                         m_display_manager.screen_center());
  m_game_objects.push_back(m_player);
  spawn_asteroids(STARTING_ASTEROIDS);
  m_asteroid_count = STARTING_ASTEROIDS;
}

bool Game::load_resources(std::string game_bgm, std::string collision_sfx,
                          std::string ship_gun_sfx, std::string game_font) {
  if (!m_game_state.load_resources(game_bgm) ||
      !m_collision_manager.load_resources(collision_sfx) ||
      !m_player->load_resources(ship_gun_sfx) ||
      !m_display_manager.load_resources(game_font)) {
    return false;
  }
  m_ship_gun_sfx = ship_gun_sfx;
  return true;
}

bool Game::is_running() const {
  return m_game_state.is_running();
}

void Game::process_input(float dt) {
  sf::Event event;
  while (m_display_manager.poll_event(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        m_game_state.close_game();
        break;
      case sf::Event::LostFocus:
        if (m_game_state.in_game()) {
          m_game_state.pause_game();
        }
        break;
      case sf::Event::Resized:
        if (m_game_state.in_game()) {
          m_game_state.pause_game();
        }
        break;
      case sf::Event::KeyReleased:
        m_game_state.update_game_state(event.key.code);
        break;
      default:
        break;
    }
  }
  if (m_game_state.in_game()) {
    m_player->control_ship(dt);
  }
}

void Game::update(float dt) {
  if (m_game_state.load()) {
    m_game_objects.erase(m_game_objects.begin() + 1U, m_game_objects.end());
    m_next_object_id = static_cast<unsigned int>(m_game_objects.size());
    spawn_asteroids(STARTING_ASTEROIDS + m_difficulty);
    m_asteroid_count = STARTING_ASTEROIDS + m_difficulty;
    m_game_state.start_game();
  } else if (m_game_state.in_game()) {
    for (auto object : m_game_objects) {
      object->update(dt);
    }
    std::vector<std::shared_ptr<GameObject>> new_objects;
    GameObject::ObjectType collider_type;
    for (auto object : m_game_objects) {
      collider_type = m_collision_manager.collision_check(*object,
                                                          m_game_objects);
      if (collider_type != GameObject::NullType) {
        object->collide();
        if (*object == GameObject::BulletType &&
            std::dynamic_pointer_cast<Bullet>(object)->get_parent_type() ==
              GameObject::PlayerType) {
          if (collider_type == GameObject::AsteroidType) {
            m_player->increment_score(Asteroid::SCORE_VALUE);
          } else if (collider_type == GameObject::SaucerType) {
            m_player->increment_score(Saucer::SCORE_VALUE);
          }
        }
      }
      if (*object == GameObject::SaucerType) {
        std::dynamic_pointer_cast<Saucer>(object)->aim(m_player->get_position());
      }
      if ((*object == GameObject::PlayerType ||
           *object == GameObject::SaucerType) &&
          object->is_shooting()) {
        new_objects.push_back(object->spawn_child(m_next_object_id++));
      } else if (*object == GameObject::AsteroidType &&
                 object->is_destroyed()) {
        if (object->get_radius() > S_ASTEROID) {
          new_objects.push_back(object->spawn_child(m_next_object_id++, 90.0F));
          new_objects.push_back(object->spawn_child(m_next_object_id++, -90.0F));
          m_asteroid_count++;
        } else if (object->get_radius() < M_ASTEROID) {
          m_asteroid_count--;
        }
      }
      if (m_display_manager.off_camera(object->get_position(),
                                       object->get_radius())) {
        if (*object != GameObject::SaucerType) {
          m_display_manager.wrap_object(*object);
        } else {
          object->collide();
        }
      }
    }
    if (m_saucer_timer <= 0.0F) {
      sf::Vector2f position = m_display_manager.saucer_spawn_position();
      float rotation = 0.0F;
      if (position.y > m_display_manager.screen_center().y) {
        rotation = 180.0F;
      }
      std::shared_ptr<Saucer> new_saucer = std::make_shared<Saucer>(
        m_next_object_id++,
        position, rotation
      );
      new_saucer->load_resources(m_ship_gun_sfx);
      new_objects.push_back(new_saucer);
      m_saucer_timer = SAUCER_INTERVAL;
    } else {
      m_saucer_timer -= dt;
    }
    m_game_objects.insert(m_game_objects.end(), new_objects.begin(),
                          new_objects.end());
    m_game_objects.erase(std::remove_copy_if(m_game_objects.begin() + 1U,
                                             m_game_objects.end(),
                                             m_game_objects.begin() + 1U,
                                             [](std::shared_ptr<GameObject> obj)
                                             { return obj->is_destroyed(); }),
                         m_game_objects.end());
    if (m_asteroid_count == 0U) {
      m_game_state.next_level();
      m_difficulty++;
      m_player->reset_ship();
    }
    if (m_player->is_destroyed()) {
      m_game_state.end_game();
    }
  } else if (m_game_state.title_screen()) {
    for (auto object : m_game_objects) {
      if (*object == GameObject::AsteroidType) {
        object->update(dt);
      }
    }
  } else if (m_game_state.reset()) {
    reset_game();
  }
  m_display_manager.draw_screen(m_game_state, dt, m_game_objects,
                                m_difficulty + 1);
}

void Game::spawn_asteroids(unsigned int asteroid_count) {
  std::shared_ptr<Asteroid> new_asteroid;
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    new_asteroid = std::make_shared<Asteroid>(m_next_object_id++, L_ASTEROID,
        m_display_manager.valid_asteroid_position(m_game_objects),
        static_cast<float>(rand() % 360U));
    m_game_objects.push_back(new_asteroid);
  }
}

void Game::reset_game() {
  m_difficulty = 0U;
  m_saucer_timer = SAUCER_INTERVAL;
  m_game_state.reset_game_state();
  m_player->reset_lives();
  m_player->reset_score();
  m_player->reset_ship();
  m_game_objects.erase(m_game_objects.begin() + 1U, m_game_objects.end());
  m_next_object_id = static_cast<unsigned int>(m_game_objects.size());
  spawn_asteroids(STARTING_ASTEROIDS);
  m_asteroid_count = STARTING_ASTEROIDS;
}

}
