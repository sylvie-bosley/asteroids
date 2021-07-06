#ifndef ASTEROIDS_GAME_CODE_INCLUDE_GAME_H
#define ASTEROIDS_GAME_CODE_INCLUDE_GAME_H

#include "spaceship.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ag {
  class Game {
  public:

    Game();
    ~Game();

    void pause();
    void resume();
    void process_input(const sf::Event::KeyEvent &key);
    bool update();
    void render(sf::RenderWindow *window_p);
    void game_over();

    bool running;

  private:

    enum GameState {TitleScreen, InGame, Paused};

    bool play_bgm();

    GameState game_state;
    ag::Spaceship player;
    sf::Music bgm;
    sf::SoundBuffer sfx_buffer;
    sf::Sound sfx;
  };
}

#endif
