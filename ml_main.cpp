#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"


int main(int argc, char const *argv[])
{
  sf::Clock global_clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "WIP");

  unsigned int mass_point_num = 1000;
  float mass = 5.0f;
  float time_increment = 0.00001;
  float tension = 100.0f;
  float length = 1.25f;
  float simulation_time = 0.0;
  sf::Vector2f initial_pos(0.0f,400.0f);
  Constraint constraint_a(true, true);
  Constraint constraint_b(true, true);
  sf::Color color(150,0,0);
  FFT fft;
  int sampling_counter = 1;


  //amplitude, width, repetitions
  Linear_Shape shape("sine", 1.0f, 1.0f, 1.0f);

  Rope rope(mass_point_num, mass, tension, length, initial_pos, constraint_a, constraint_b, color);

  rope.set_shape(shape, 0.5f);

  sf::Time global_time;
  sf::Time time;
  sf::Clock clock;
  
  clock.restart();
  global_clock.restart();
  //________________________________________________________________________
  //main cicle
  while(window.isOpen())
  {
    // std::cout << "tempo: " << global_clock.getElapsedTime().asSeconds() << std::endl;
    // std::cout << "simulation_time: " << simulation_time << std::endl;

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
      simulation_time += time_increment;
    }

  //! Entering FFT only mode
  #ifdef FFT_ONLY
  
    if(simulation_time > TIME_SAMPLING * sampling_counter)
    {
      for(unsigned int i=0; i<rope.size(); ++i)
      {
        if(std::remainder(i, (mass_point_num/POS_SAMPLING)) == 0.0f)
          fft.input(rope.get_position_at(i), simulation_time);
      }
      sampling_counter++; 
    }
    
  //! Entering graphic only mode
  #else 

    window.clear();

    for(unsigned int i=0; i<rope.size(); ++i)
      window.draw(rope.get_mass(i).point);

    window.display();

  #endif
  }

  return 0;
}