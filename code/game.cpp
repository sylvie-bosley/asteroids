#include "include/spaceship.h"
#include "include/game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ag {
  Game::Game() : player{} {
    running = true;
    game_state = TitleScreen;
  }

  Game::~Game() {
  }

  void Game::pause() {
    if (game_state == InGame)
      game_state = Paused;
  }

  void Game::resume() {
    if (game_state == Paused)
      game_state = InGame;
  }

  void Game::process_input(const sf::Event::KeyEvent &key) {
    switch (key.code) {
    case sf::Keyboard::Key::Enter:
      if (game_state == TitleScreen)
        game_state = InGame;
      break;

    case sf::Keyboard::Key::Space:
      if (game_state == InGame)
        player.fire();
      break;

    case sf::Keyboard::Key::Escape:
      if (game_state == InGame)
        game_state = Paused;
      break;

    case sf::Keyboard::Key::Up:
      if (game_state == InGame)
      player.thrust();
      break;

    // TODO: Handle turning
    default:
      break;
    }
  }

  bool Game::update() {
    bool updated = true;

    // TODO: Basically everything
    if (bgm.getStatus() == sf::Music::Stopped)
      if (!play_bgm())
        updated = false;

    return updated;
  }

  void Game::render(sf::RenderWindow *window_p) {
    window_p->clear(sf::Color::Black);

    // TODO: This is placeholder code to test functionality. This logic needs to
    // be moved to the correct places and actual rendering logic added here
    sf::CircleShape ship(10.f, 3);
    ship.setOutlineThickness(1.f);
    ship.setFillColor(sf::Color::Black);
    ship.setOutlineColor(sf::Color::White);
    ship.setPosition(player.position.x, player.position.y);
    window_p->draw(ship);

    window_p->display();
  }

  void Game::game_over() {
    running = false;
  }

  bool Game::play_bgm() {
    bool playing = true;

    // TODO: This music file is a placeholder
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
}
