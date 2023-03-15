#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_ft.h"

int main(int argv, char** argc)
{
  sf::Clock global_clock;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "fourier analysis");

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
  float sampling_time = 0.0f;
  float sampling_time_increment = 0.0f;
  int fourier_bins_number = 32;

  Rope rope(mass_point_num, mass, tension, length, initial_pos, constraint_a, constraint_b, color);

  Linear_Shape shape("sine", 1.0f, 1.0f, 0.5f);
  rope.set_shape(shape, 0.5f);

  DFT dft;
  std::vector<sf::CircleShape> fourier_transform;

  {
  sf::CircleShape temp;
  temp.setRadius(DEFAULT_P_RADIUS);
  temp.setPointCount(DEFAULT_C_POINT_COUNT);
  temp.setOrigin(DEFAULT_P_RADIUS / 2 , DEFAULT_P_RADIUS / 2);
  temp.setPosition(0.0f,0.0f);
  temp.setFillColor(color);
  
  for(int i=0; i<fourier_bins_number/2; ++i)
    fourier_transform.push_back(temp);

  int i;
  for(i = 0;; ++i)
    if(fourier_transform.size() < pow(2, i))
    {
      --i;
      break;
    }


  while(pow(2,i) < fourier_transform.size())
    fourier_transform.pop_back();
  
  }

  global_clock.restart();

  //* main cycle
  
  while(window.isOpen())
  { 
    clock.restart();

    system("clear");
    std::cout << simulation_time << "\t Simulation time" << std::endl 
              << sampling_time << "\t \t next samplingation time" << std::endl 
              << global_clock.getElapsedTime().asSeconds() << "\t \t real time elapsed" << std::endl;
    
    std::cout << std::endl;

    if(simulation_time >= sampling_time)
    {
      sampling_time += sampling_time_increment;

      std::vector<sf::Vector2f> f_position;

      for(int i=0; i<fourier_bins_number; ++i)
        f_position.push_back(rope.get_position_at(static_cast<int>(i * rope.size() / fourier_bins_number)));
      
      dft.input(f_position, simulation_time);

      f_position = dft.evaluate_at_time(simulation_time);

      for(int i=0; i<f_position.size()/2; ++i)
        fourier_transform.at(i).setPosition(f_position.at(i));
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
//    for(int i=0; i<rope.size(); ++i)
//      window.draw(rope.get_mass(i).point);
    for(int i=0; i<fourier_transform.size(); ++i)
      window.draw(fourier_transform.at(i));
    window.display();
  }

  return 0;
}