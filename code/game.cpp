#define DEBUG

#include "include/game.h"

#include <string>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/asteroid.h"
#include "include/spaceship.h"
#include "include/helpers.h"

namespace ag {

Game::Game()
    : game_window{sf::VideoMode(DISPLAY_SIZE.x, DISPLAY_SIZE.y), "Asteroids"},
      player{} {
  starting_asteroids = 4U;
  difficulty = 0U;
  generate_asteroids(starting_asteroids);
  running = true;
  game_state = TitleScreen;
}

Action Game::process_input() {
  Action action;
  sf::Event event;
  while (game_window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      running = false;
      game_window.close();
      break;
    case sf::Event::LostFocus:
      game_state = Paused;
      break;
    case sf::Event::Resized:
      game_state = Paused;
      break;
    case sf::Event::KeyPressed:
      action = parse_player_action(event.key.code);
      break;
    default:
      break;
    }
  }
  return action;
}

bool Game::update(const Action action, const sf::Time dt) {
  if (bgm.getStatus() == sf::Music::Stopped) {
    if (!play_bgm()) {return false;}
  }

  switch (game_state) {
    case TitleScreen: {
      if (action == Select) {
        game_state = InGame;
      } else if (action == Escape) {
        running = false;
        game_window.close();
      }
      break;
    }
    case GameOver: {
      if (action == Select) {
        reset_game();
      }
      break;
    }
    case Paused: {
      if (action == Select) {
        game_state = InGame;
      } else if (action == Escape) {
        reset_game();
      }
      break;
    }
    case InGame: {
      if (action == Escape) {
        game_state = Paused;
      } else if (action != Select && action != Unused) {
        player.control_ship(action, dt);
      }
      player.update_pos();
      for (unsigned int i = 0U; i < (starting_asteroids + difficulty); ++i) {
        asteroids[i].update_pos(dt);
      }
      break;
    }
  }
  return true;
}

void Game::render() {
  game_window.clear(sf::Color::Black);

  // TODO: This is placeholder code to test functionality. This logic needs to
  // be moved to the correct places and actual rendering logic added here
#ifdef DEBUG
  sf::CircleShape ship(10.0F, 3U);
  ship.setOrigin(10.0F, 10.0F);
  ship.setOutlineThickness(1.0F);
  ship.setFillColor(sf::Color::Black);
  ship.setOutlineColor(sf::Color::White);
  ship.setPosition(player.position);
  ship.setRotation(-player.orientation);

  sf::Font font;
  font.loadFromFile("data/test/sansation.ttf");
  sf::Text ship_stats;
  ship_stats.setFont(font);

  std::string velocity_x_str = std::to_string(player.velocity.x);
  std::string velocity_y_str = std::to_string(player.velocity.y);
  std::string rotation_str = std::to_string(player.orientation);

  std::string stats_str = "X Velocity: " + velocity_x_str + "\n" +
                          "Y Velocity: " + velocity_y_str + "\n" +
                          "Rotation: " + rotation_str;

  ship_stats.setString(stats_str);
  ship_stats.setCharacterSize(20U);
  ship_stats.setFillColor(sf::Color::White);
  ship_stats.setPosition(5.0F, 5.0F);

  game_window.draw(ship_stats);
  game_window.draw(ship);
  for (unsigned int i = 0U; i < (starting_asteroids + difficulty); ++i) {
    game_window.draw(asteroids[i].sprite);
  }
#endif

  game_window.display();
}

bool Game::is_running() {
  return running;
}

void Game::generate_asteroids(const unsigned int asteroid_count) {
  for (unsigned int i = 0U; i < asteroid_count; ++i) {
    asteroids.push_back(Asteroid{});
  }
}

void Game::reset_game() {
  game_state = TitleScreen;
  player.reset_ship();
  asteroids.clear();
  generate_asteroids(starting_asteroids);
}

bool Game::play_bgm() {
  bool playing = true;
  switch (game_state) {
    case TitleScreen:
      if (!bgm.openFromFile("data/test/orchestral.ogg"))
        playing = false;
      break;
    case InGame:
      if (!bgm.openFromFile("data/test/orchestral.ogg"))
        playing = false;
      break;
    case Paused:
      if (!bgm.openFromFile("data/test/orchestral.ogg"))
        playing = false;
      break;
    default:
      break;
  }
  if (playing) {
    bgm.setLoop(true);
    bgm.play();
  }
  return playing;
}

Action Game::parse_player_action(const sf::Keyboard::Key key) {
  Action action;
  switch (key) {
    case sf::Keyboard::Key::Escape:
      action = Escape;
      break;
    case sf::Keyboard::Key::Enter:
      action = Select;
      break;
    case sf::Keyboard::Key::Space:
      action = FireGun;
      break;
    case sf::Keyboard::Key::Up:
      action = Up;
      break;
    case sf::Keyboard::Key::Down:
      action = Down;
      break;
    case sf::Keyboard::Key::Left:
      action = Left;
      break;
    case sf::Keyboard::Key::Right:
      action = Right;
      break;
    default:
      action = Unused;
      break;
  }
  return action;
}

}
