#include "include/game.h"
#include "include/helpers.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main() {
  ag::Game game;
  ag::Action action;
  sf::Clock frame_clock;

  while (game.running) {
    sf::Time dt = frame_clock.restart();
    action = game.process_input();

    if (!game.update(action, dt)) {
      // TODO: Error handling
      return 1;
    }

    game.render();
  }

  return 0;
}
