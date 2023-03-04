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
  //system("rm results.dat");
  sf::Clock global_clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "Simulation of transmission & reflection from a more dense to a less dense rope");

  unsigned int mass_point_num = 2000;
  float mass1 = 5.0f;
  float mass2 = 1.0f;
  float time_increment = 0.00001f;
  float tension = 100.0f;
  float length = 0.625f;
  sf::Vector2f initial_pos1(0.0f,400.0f);
  sf::Vector2f initial_pos2(500.0f,400.0f);
  Constraint constraint_a(true, false);
  Constraint constraint_b(true, false);
  sf::Color color1(150,0,0);
  sf::Color color2(0,0,150);

  //amplitude, width, repetitions
  Linear_Shape shape("wave", 1.0f, 0.1f, 1.0f);
  Linear_Shape line("line");

  Rope rope1(mass_point_num, mass1, tension, length, initial_pos1, constraint_a, constraint_b, color1);
  Rope rope2(mass_point_num, mass2, tension, length, initial_pos2, constraint_a, constraint_b, color2);

  rope1.set_shape(shape, 0.5f);
  rope2.set_shape(line,0.0f);

  Rope rope(&rope1, &rope2);

  sf::Time global_time;
  sf::Time time;
  sf::Clock clock;
  
  clock.restart();
  global_clock.restart();
  //_________________________________________________________________________________________________________
  //main cicle
  while(window.isOpen())
  {
    time = clock.getElapsedTime(); 
    clock.restart();

    global_time = global_clock.getElapsedTime();    
    
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