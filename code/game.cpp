#include "include/game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/asteroid.h"
#include "include/spaceship.h"
#include "include/helpers.h"

namespace ag {

Game::Game(const sf::Font &font)
    : game_window{sf::VideoMode(DISPLAY_SIZE.x, DISPLAY_SIZE.y), "Asteroids"},
      player{font} {
  starting_asteroids = 4U;
  difficulty = 0U;
  generate_asteroids(starting_asteroids);
  running = true;
  game_state = TitleScreen;
}

bool Game::is_running() {
  return running;
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

bool Game::update(const Action action, const sf::Time &dt) {
  if (bgm.getStatus() == sf::Music::Stopped) {
    if (!play_bgm()) {return false;}
  }

  switch (game_state) {
    case TitleScreen: {
      if (action == Enter) {
        game_state = InGame;
      } else if (action == Escape) {
        running = false;
        game_window.close();
      }
      break;
    }
    case GameOver: {
      if (action == Enter) {
        reset_game();
      }
      break;
    }
    case Paused: {
      if (action == Enter) {
        game_state = InGame;
      } else if (action == Escape) {
        reset_game();
      }
      break;
    }
    case InGame: {
      if (action == Escape) {
        game_state = Paused;
      } else if (action != Enter && action != Unused) {
        player.control_ship(action, dt);
      }
      player.update();
      for (unsigned int i = 0U; i < (starting_asteroids + difficulty); ++i) {
        asteroids[i].update(dt);
      }
      break;
    }
  }
  return true;
}

void Game::render() {
  game_window.clear(sf::Color::Black);
  game_window.draw(player.get_ship_stats());
  game_window.draw(player.get_sprite());
  for (unsigned int i = 0U; i < (starting_asteroids + difficulty); ++i) {
    game_window.draw(asteroids[i].sprite);
  }
  game_window.display();
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
      action = Enter;
      break;
    case sf::Keyboard::Key::Space:
      action = Space;
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
