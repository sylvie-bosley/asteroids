#include "include/asteroids.h"
#include "include/helpers.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main(int argc, char *argv[]) {
  sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  sf::Font font;
  if (!font.loadFromFile("data/test/sansation.ttf"))
    return 1;
  sf::Text text;
  text.setFont(font);

  if (argc > 1)
    text.setString("Hello " + Asteroids::capitalize(argv[1]) + "!");
  else
    text.setString("Hello user!");

  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Red);
  text.setStyle(sf::Text::Bold | sf::Text::Underlined);
  float x_pos = 100.f - (text.getGlobalBounds().width / 2.f);
  float y_pos = 100.f - (text.getGlobalBounds().height);
  text.setPosition(x_pos, y_pos);

  sf::Music orchestral;
  if (!orchestral.openFromFile("data/test/orchestral.ogg"))
    return 1;
  orchestral.setLoop(true);

  sf::SoundBuffer ball_buffer;
  sf::Sound ball;
  if (!ball_buffer.loadFromFile("data/test/ball.wav"))
    return 1;
  ball.setBuffer(ball_buffer);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (orchestral.getStatus() == sf::Sound::Stopped)
      orchestral.play();

    window.clear();
    window.draw(shape);
    window.draw(text);
    window.display();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
        ball.getStatus() == sf::Sound::Stopped) {
      ball.play();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      break;
  }

  return 0;
}
