#include "include/main.h"
#include "include/helpers.h"
#include "include/game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main() {
  ag::Game game;
  sf::RenderWindow window{sf::VideoMode(ag::DISPLAY_SIZE.x, ag::DISPLAY_SIZE.y),
                          "Asteroids"};
  // TODO: Maybe this should be in a Display object?

  // TODO: Clean all this code up. It shouldn't be here.
  sf::Music bgm;
  if (!bgm.openFromFile("data/test/orchestral.ogg"))
    return 1;
  bgm.setLoop(true);

  sf::SoundBuffer sound_effect_buffer1;
  sf::SoundBuffer sound_effect_buffer2;
  sf::Sound sound_effect;
  if (!sound_effect_buffer1.loadFromFile("data/test/ball.wav"))
    return 1;
  sound_effect.setBuffer(sound_effect_buffer1);
  // END TODO

  while (game.running) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        game.game_over();
        break;

      case sf::Event::LostFocus:
        game.pause();
        break;

      case sf::Event::KeyPressed:
        game.process_input(event.key);
        break;

      default:
        break;
      }
    }

    if (!game.update()) {
      // TODO: Error handling
      return 1;
    }

    game.render(&window);
  }

  return 0;
}
