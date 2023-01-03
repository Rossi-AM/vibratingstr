#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


int main()
{
  sf::RenderWindow window(sf::VideoMode(1000, 600), "when will ale-senpai notice me");

  sf::CircleShape c1(5.0,10);
  sf::CircleShape c2(5.0,10);
  
  c1.setPosition(100, 300);
  c2.setPosition(900, 300);

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    window.clear();
    window.draw(c1);
    window.draw(c2);
    window.display();
  }

  return 0;
}