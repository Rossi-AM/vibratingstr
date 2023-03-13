#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

//!________________________________________________________________________________________________________
//! MAIN

int main(int argc, char const *argv[])
{
  sf::RenderWindow window(sf::VideoMode(1000, 800), "Simulation of a wave moving on a less massive rope (mass = 1)");

  unsigned int mass_point_num = 4000;
  float mass = 1.0f;
  float time_increment = 0.00001f;
  float tension = 100.0f;
  float length = 1.25f;
  sf::Vector2f initial_pos(0.0f,400.0f);
  Constraint constraint_a(true, false);
  Constraint constraint_b(true, true);
  sf::Color color(0,0,150);

  //amplitude, width, repetitions
  Linear_Shape shape("wave", 1.0f, 0.05f, 1.0f);

  Rope rope(mass_point_num, mass, tension, length, initial_pos, constraint_a, constraint_b, color);

  rope.set_shape(shape, 0.5f);

  sf::Clock clock;
  
  clock.restart();
  //_________________________________________________________________________________________________________
  //main cicle
  while(window.isOpen())
  {
    clock.restart();
    
    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    while(clock.getElapsedTime() < sf::milliseconds(33))
    {
      rope.apply();
      rope.update(time_increment);
    }

    window.clear();
    for(unsigned int i=0; i<rope.size(); ++i)
      window.draw(rope.get_mass(i).point);
    window.display();
  }

  return 0;
}