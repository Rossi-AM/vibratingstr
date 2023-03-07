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
  sf::RenderWindow window(sf::VideoMode(1000, 800), "Wave travelling with horizontal motion allowed (red) and not allowed (blue)");

  unsigned int mass_point_num = 2000;
  float mass_1 = 1.0f;
  float mass_2 = 1.0f;
  float time_increment = 0.00001f;
  float tension = 100.0f;
  float length = 1.25;
  sf::Vector2f initial_pos_1(0.0f, 250.0f);
  sf::Vector2f initial_pos_2(0.0f, 550.0f);
  Constraint constraint_a(true, false);
  Constraint constraint_b(true, true);
  sf::Color color_1(150,0,0);
  sf::Color color_2(0,0,150);

  //amplitude, width, repetitions
  Linear_Shape shape("wave", 1.0f, 0.1f, 1.0f);

  Rope rope_1(mass_point_num, mass_1, tension, length, initial_pos_1, constraint_a, constraint_b, color_1);
  Rope rope_2(mass_point_num, mass_2, tension, length, initial_pos_2, constraint_a, constraint_b, color_2);

  rope_1.set_shape(shape, 0.33f);
  rope_2.set_shape(shape, 0.33f);

  rope_1.set_x_sliding(true);
  rope_2.set_x_sliding(false);

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
      rope_1.apply();
      rope_1.update(time_increment);

      rope_2.apply();
      rope_2.update(time_increment);
    }

    window.clear();
    for(unsigned int i=0; i<rope_1.size(); ++i)
      window.draw(rope_1.get_mass(i).point);
    for(unsigned int i=0; i<rope_2.size(); ++i)
      window.draw(rope_2.get_mass(i).point);
    window.display();
  }

  return 0;
}