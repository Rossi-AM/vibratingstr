//to compile use "c++ *.cpp -c && c++ *.o -o ex_main -lsfml-graphics\ -lsfml-window\ -lsfml-system && ./ex_main && rm *.o && clear"
//make sure that sfml is installed, in case follow the instruction at https://www.sfml-dev.org/download/sfml/2.5.1/

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

int main(int argv, char** argc)
{
  sf::Clock global_clock;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "vibrating string");

  unsigned int mass_point_num = 1000;
  float mass = 5.0f;
  float time_increment = 0.00001f;
  float tension = 200.0f;
  float length = 1.25f;
  sf::Vector2f initial_pos(0.0f,static_cast<float>(window.getSize().y) / 2.0f);
  Constraint constraint_a(true, true);
  Constraint constraint_b(true, true);
  sf::Color color(150,0,0);

  float simulation_time = 0.0f;
  float i_campion_time = 0.0f;
  float i_campion_time_increment = 0.01f;
  float t_campion_time = 0.35f;
  bool t_to_print = true;
  int index_to_print = 5000;

  Rope rope(mass_point_num, mass, tension, length, initial_pos, constraint_a, constraint_b, color);

  Linear_Shape shape("sine", 1.0f, 1.0f, 0.5f);
  rope.set_shape(shape, 0.5f);

  global_clock.restart();

  std::fstream time_print("shape_at_time.dat", std::ios::app);
  std::fstream index_print("positions_at_times.dat", std::ios::app); 
  if(!time_print.good() || !index_print.good())
  {
    return 1;
  }

  index_print << index_to_print << std::endl;

  //* main cycle
  
  while(window.isOpen())
  { 
    clock.restart();

    system("clear");
    std::cout << simulation_time << "\t simulation time" << std::endl 
              << i_campion_time << "\t \t next campionation time" << std::endl 
              << t_campion_time << "\t \t campionation time for the rope shape" << std::endl
              << global_clock.getElapsedTime().asSeconds() << "\t \t real time elapsed" << std::endl; 

    if(simulation_time > i_campion_time)
    {
      i_campion_time += i_campion_time_increment;
      float y_pos = rope.get_position_at(index_to_print).y - rope.get_position_at(0).y;
      index_print << y_pos << ' ' << simulation_time << std::endl; 
    }

    if(simulation_time > t_campion_time && t_to_print)
    {
      time_print << simulation_time << std::endl;
      for(int i = 0; i<rope.size(); ++i)
        time_print << rope.get_position_at(i).y << ' ' << rope.get_position_at(i).x << std::endl;
      t_to_print = false;
    }

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
  
    window.clear();
    for(int i=0; i<rope.size(); ++i)
      window.draw(rope.get_mass(i).point);
    window.display();
  }

  time_print.close();
  index_print.close();

  return 0;
}