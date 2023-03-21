#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_ft.h"

#if FT
  #define window_name "Fourier Analysis"
#else
  #define window_name "Vibrating string"
#endif

int main(int argv, char** argc)
{
  // GUI

  bool chosen = false;
  std::string shape_name = DEFAULT_CURRENT_FUNCTION;
  float shape_normal_mode = DEFAULT_REPETITIONS;

  while(!chosen)
  {
  system("clear");
  std::cout << "\t +--------------------------------+" << std::endl
            << "\t --- Vibrating string simulator ---" << std::endl
            << "\t +--------------------------------+" << std::endl;
  std::cout << "\nChoose a shape between" << std::endl
            << "1. sine" << std::endl
            << "2. cosine" << std::endl
            << "3. square" << std::endl
            << "4. triangle" << std::endl
            << "\nType the chosen shape: ";
  std::cin >> shape_name;
  if(shape_name == "sine" || shape_name == "cosine" || 
     shape_name == "square" || shape_name == "triangle")
      chosen = true;
  }

  std::cout << "\n\nWhat normal mode do you want to simulate? ";
  std::cin >> shape_normal_mode;

  std::cout << "Computing..." << std::endl;

  // Variables initialization

  sf::Clock global_clock;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), window_name);

  unsigned int mass_point_num = 1000;
  float mass = 5.0f;
  float time_increment = 0.00001f;
  float simulation_time = 0.0f;
  float tension = 200.0f;
  float length = 1.25f;
  sf::Vector2f initial_pos(0.0f,static_cast<float>(window.getSize().y) / 2.0f);
  Constraint constraint_a(true, true);
  Constraint constraint_b(true, true);
  sf::Color color(150,0,0);

  // Constructing simulation elements 

  Rope rope(mass_point_num, mass, tension, length, initial_pos, constraint_a, constraint_b, color);

  Linear_Shape shape(shape_name, 1.0f, 1.0f, shape_normal_mode/2);
  rope.set_shape(shape, 0.5f);

  // Constructing DFT elements

  #if FT

  float sampling_time = 0.0f;
  float sampling_time_increment = 0.0f;
  int fourier_bins_number = 32;
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
  }
  
  #endif


  global_clock.restart();

  //* Main cycle
  
  while(window.isOpen())
  { 
    clock.restart();


    // Gathering data for the DFT
    #ifdef FT

    if(simulation_time >= sampling_time)
    {
      sampling_time += sampling_time_increment;

      std::vector<sf::Vector2f> f_position;

      for(int i=0; i<fourier_bins_number; ++i)
        f_position.push_back(rope.get_position_at(static_cast<int>(i * rope.size() / fourier_bins_number)));
      
      
      dft.input(f_position, simulation_time);

      f_position = dft.evaluate_at_time(simulation_time, DEFAULT_NORMAL_HEIGHT, true);

      for(int i=0; i<f_position.size()/2; ++i)
        fourier_transform.at(i).setPosition(f_position.at(i));
    }

    #endif

    // SFML window handling
    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    // Updating the simulation
    while(clock.getElapsedTime() < sf::milliseconds(33))
    {
      rope.apply();
      rope.update(time_increment);

      simulation_time += time_increment;
    }

    // Drawning the results
    window.clear();
    
    #ifdef FT
    
    for(int i=0; i<fourier_transform.size(); ++i)
      window.draw(fourier_transform.at(i));

    #else

    for(int i=0; i<rope.size(); ++i)
      window.draw(rope.get_mass(i).point);
    
    #endif

    window.display();
  }

  system("clear");

  return 0; 
}