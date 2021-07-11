#include "include/spaceship.h"
#include "include/game.h"
#include "include/helpers.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

namespace ag {

Game::Game()
    : player{sf::Vector2i{dspl_size.x / 2, dspl_size.y / 2}},
      game_window{sf::VideoMode(dspl_size.x, dspl_size.y), "Asteroids"} {
  running = true;
  game_state = TitleScreen;
}

Game::~Game() {
}

void Game::pause_game() {
  if (game_state == InGame)
    game_state = Paused;
}

void Game::resume() {
  if (game_state == Paused)
    game_state = InGame;
}

Action Game::process_input() {
  Action action;
  sf::Event event;
  while (game_window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      close_game();
      break;
    case sf::Event::LostFocus:
      pause_game();
      break;
    case sf::Event::Resized:
      pause_game();
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
    case TitleScreen:
      if (action == Select) {
        game_state = InGame;
      } else if (action == Escape) {
        close_game();
      }
      break;
    case GameOver:
      if (action == Select) {
        game_state = TitleScreen;
      }
      break;
    case Paused:
      if (action == Select) {
        game_state = InGame;
      } else if (action == Escape) {
        game_state = TitleScreen;
      }
      break;
    case InGame:
      if (action == Escape) {
        pause_game();
      } else if (action != Select && action != Unused) {
        player.control_ship(action, dt);
      }
      break;
  }

  player.update_pos(dspl_size);
  // TODO: Update game state

  return true;
}

void Game::render() {
  game_window.clear(sf::Color::Black);

  // TODO: This is placeholder code to test functionality. This logic needs to
  // be moved to the correct places and actual rendering logic added here
  sf::CircleShape ship(10.0f, 3);
  ship.setOrigin(10.0f, 10.0f);
  ship.setOutlineThickness(1.0f);
  ship.setFillColor(sf::Color::Black);
  ship.setOutlineColor(sf::Color::White);
  ship.setPosition(player.position.x, player.position.y);
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
  ship_stats.setCharacterSize(20);
  ship_stats.setFillColor(sf::Color::White);
  ship_stats.setPosition(5.0f, 5.0f);

  game_window.draw(ship_stats);
  game_window.draw(ship);
  game_window.display();
}

void Game::close_game() {
  running = false;
  game_window.close();
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
