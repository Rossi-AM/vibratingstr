#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <complex>
#include <iterator>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

#define FFT_ONLY

int main(int argc, char const *argv[])
{
  sf::Clock global_clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "WIP");

  unsigned int mass_point_num = 1000;
  float mass_1 = 5.0f;
  float mass_2 = 10.0f;
  float time_increment = 0.00001;
  float tension = 100.0f;
  float length = 0.625f;
  float simulation_time = 0.0;
  sf::Vector2f initial_pos1(0.0f,400.0f);
  sf::Vector2f initial_pos2(500.0f,400.0f);
  Constraint constraint_a(true, false);
  Constraint constraint_b(true, false);
  sf::Color color_1(150,0,0);
  sf::Color color_2(0,0,150);
  FFT fft;
  int sampling_counter = 1;


  //amplitude, width, repetitions
  Linear_Shape shape_1("wave", 1.0f, 0.1f, 1.0f);
  Linear_Shape shape_2("line", 1.0f, 1.0f, 1.0f);

  
  Rope rope_1(mass_point_num, mass_1, tension, length, initial_pos1, constraint_a, constraint_b, color_1);
  Rope rope_2(mass_point_num, mass_2, tension, length, initial_pos2, constraint_a, constraint_b, color_2);

  rope_1.set_shape(shape_1, 0.5f);
  rope_2.set_shape(shape_2, 0.5f);

  Rope rope(&rope_1, &rope_2);

  sf::Time global_time;
  sf::Time time;
  sf::Clock clock;
  
  clock.restart();
  global_clock.restart();
  //________________________________________________________________________
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

      // Debug
      for(unsigned int i=0; i<POS_SAMPLING; ++i)
        std::cout << fft.get_data().at(sampling_counter-2).time << "\t" << fft.get_data().at(sampling_counter-2).position.at(i).x << "\t" << fft.get_data().at(sampling_counter-2).position.at(i).y << std::endl;
    }

    float *f;   // input: f[n] = f(a + n*T/N), n=0...N-1
    std::complex *F; // output: F[k] = F(2*k*PI/T), k=0...N/2
    DFTI_DESCRIPTOR_HANDLE h = NULL;
    DftiCreateDescriptor(&h,DFTI_SINGLE,DFTI_REAL,1,(MKL_LONG)N);
    DftiSetValue(h,DFTI_CONJUGATE_EVEN_STORAGE,DFTI_COMPLEX_COMPLEX);
    DftiSetValue(h,DFTI_PLACEMENT,DFTI_NOT_INPLACE);
    DftiCommitDescriptor(h);
    DftiComputeForward(h,f, F);
    for (int k = 0; k <= POS_SAMPLING/2; ++k)
    {
    F[k] *= (mass_point_num/POS_SAMPLING)*std::complex( cos(2*M_PI*a*k/T), -sin(2*M_PI*a*k/T) );
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