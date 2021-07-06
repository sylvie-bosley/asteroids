#include "include/spaceship.h"

#include <SFML/Audio.hpp>

namespace ag {
  Spaceship::Spaceship() : position{640, 360},
                           ship_sfx_buffer1{},
                           ship_sfx_buffer2{},
                           ship_gun_sound{},
                           ship_engines_sound{}
  { }

  Spaceship::~Spaceship() {
  }

  void Spaceship::thrust() {
    position.y -= 10;
  }

  void Spaceship::fire() {
    ship_sfx_buffer1.loadFromFile("data/test/ball.wav");
    ship_gun_sound.setBuffer(ship_sfx_buffer1);
    ship_gun_sound.play();
  }

  // TODO: So many things
}
