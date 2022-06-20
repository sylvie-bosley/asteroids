#include <string>

#include <SFML/System.hpp>

#include "game.h"
#include "helpers.h"

int main() {
  std::srand(std::time(nullptr));
  ag::Game game{};
  std::string game_bgm_file = "res/orchestral.ogg";
  std::string collision_sfx_file = "res/boom.wav";
  std::string ship_gun_sfx_file = "res/gun.wav";
  std::string game_font_file = "res/sansation.ttf";
  if (!game.load_resources(game_bgm_file, collision_sfx_file, ship_gun_sfx_file,
                           game_font_file)) {
    return 1;
  }
  sf::Clock frame_clock;
  sf::Time dt;
  do {
    dt = frame_clock.restart();
    game.process_input(dt.asSeconds());
    game.update(dt.asSeconds());
  } while (game.is_running());
  return 0;
}
