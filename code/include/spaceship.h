#ifndef ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H
#define ASTEROIDS_GAME_CODE_INCLUDE_SPACESHIP_H

#include <SFML/Audio.hpp>

namespace ag {
  class Spaceship {
  public:

    Spaceship();
    ~Spaceship();

    void thrust();
    void fire();

    sf::Vector2i position;

  private:

    sf::SoundBuffer ship_sfx_buffer1;
    sf::SoundBuffer ship_sfx_buffer2;
    sf::Sound ship_gun_sound;
    sf::Sound ship_engines_sound;
  };
}

#endif
