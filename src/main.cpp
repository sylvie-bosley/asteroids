#include <SFML/System.hpp>

#include "game.h"
#include "helpers.h"

int main() {
  std::srand(std::time(nullptr));
  ag::Game game{};
  std::string title_bgm_file = "res/orchestral.ogg";
  std::string game_bgm_file = "res/orchestral.ogg";
  std::string gameover_bgm_file = "res/orchestral.ogg";
  std::string ship_gun_sfx_file = "res/ball.wav";
  std::string game_font_file = "res/sansation.ttf";
  if (!game.load_resources(title_bgm_file, game_bgm_file, gameover_bgm_file,
                           ship_gun_sfx_file, game_font_file)) {
    return 1;
  }
  sf::Clock frame_clock;
  sf::Time dt;
  do {
    dt = frame_clock.restart();
    game.process_input();
    if (!game.update(dt)) {
      // TODO: Error handling
      return 1;
    }
    game.render();
  } while (game.is_running());
  return 0;
}
