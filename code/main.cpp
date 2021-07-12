#include <SFML/System.hpp>

#include "include/game.h"
#include "include/helpers.h"

int main() {
  std::srand(std::time(nullptr));
  ag::Game game{};
  ag::Action action;
  sf::Clock frame_clock;
  do {
    sf::Time dt = frame_clock.restart();
    action = game.process_input();
    if (!game.update(action, dt)) {
      // TODO: Error handling
      return 1;
    }
    game.render();
  } while (game.is_running());
  return 0;
}
