#include <SFML/System.hpp>

#include "include/game.h"
#include "include/helpers.h"

int main() {
  std::srand(std::time(nullptr));
  ag::Game game{};
  std::string title_bgm_file = "data/test/orchestral.ogg";
  std::string game_bgm_file = "data/test/orchestral.ogg";
  std::string gameover_bgm_file = "data/test/orchestral.ogg";
  std::string ship_gun_sfx_file = "data/test/ball.wav";
  std::string ship_stats_font_file = "data/test/sansation.ttf";
  if (!game.load_resources(title_bgm_file, game_bgm_file, gameover_bgm_file,
                           ship_gun_sfx_file, ship_stats_font_file)) {
    return 1;
  }
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
