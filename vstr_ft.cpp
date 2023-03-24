//
// 03/2023
//                  
// Rossi A. M.:  alessandromatteo.rossi@studenti.unimi.it
// Tambini M. :  marco.tambini@studenti.unimi.it
//  

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_ft.h"

//?___________________________________________________________________________________________________________
//? DFT

void 
DFT::input(sf::Vector2f position, float time)
{
  // Input the position for the given time
  bool check = true;
  for(int i = 0; i<time_data.size(); ++i)
  {
    if(time == time_data.at(i).time)
    {
      time_data.at(i).position.push_back(position);
      check = false;
      break;
    }
  }

  // If the given time doesn't exist, create a new FT_DATA for such time
  if(check)
  {
    FT_DATA temp;
    temp.position.push_back(position);
    temp.time = time;
    time_data.push_back(temp);
  }
}


void 
DFT::input(std::vector<sf::Vector2f> position, float time)
{
  for(int i=0; i<position.size(); ++i)
    input(position.at(i), time);
}


std::vector<sf::Vector2f>
DFT::evaluate_at_time(float time, float y0, bool to_normalize, float normal_height)
{
  std::vector<sf::Vector2f> result;

  // Look for the time 
  int i;
  for(i=0; i<time_data.size(); ++i)
    if(time == time_data.at(i).time) break;
  
  // If time is not found, output the FFT at the last provided time
  if(i == time_data.size())
    --i;

  result = time_data.at(i).position;

  // Compute the DFT
  evaluate(&result);

  if(to_normalize)
    normalize(&result);

  return result;
}


std::vector<FT_DATA> 
DFT::evaluate_all_time(float y0, bool to_normalize, float normal_height)
{
  std::vector<FT_DATA> result;

  // Evaluate for all times  
  for(int i=0; i<time_data.size(); ++i)
  {
    FT_DATA temp;
    temp.position = evaluate_at_time(time_data.at(i).time);
    temp.time = time_data.at(i).time;
    result.push_back(temp);
  }

  return result;
}


void
DFT::evaluate(std::vector<sf::Vector2f> *point , float y0)
{
  // If input size is odd, makes it even;
  if(point->size()%2 == 1)
    point->pop_back();

  std::vector<std::complex<double>> complex_point;

  // Load a complex vector with the input points
  for(int i = 0; i<point->size(); ++i)
  {
    std::complex<double> temp(point->at(i).y - y0, 0.0);
    complex_point.push_back(temp);
  }

  // Compute the DFT
  complex_point =  transform(complex_point);

  // Load the output with the transform
  // the first half of points is the fourier transform for a sine 
  // the second half is the fourier transform for a cosine
  for(int i=0; i<complex_point.size()/2; ++i)
  {
    point->at(i).x *= 2.0;
    point->at(i).y = y0 - abs(complex_point.at(i).imag());

    point->at(i+complex_point.size()/2).x = point->at(i).x;
    point->at(i+complex_point.size()/2).y = y0 - abs(complex_point.at(i).real());
  }
}


std::vector<std::complex<double>>
DFT::transform(std::vector<std::complex<double>> point)
{
  // The sampling step assures that every point is a normal mode of the rope
  double frequency_steps = 0.5;

  std::complex i(0.0, 1.0);

  std::vector<std::complex<double>> result;

  // Matrix-vector moltipication of original function and test functions
  for(double k=0.0, frequency = 0.0; k<point.size(); ++k)
  {
    std::complex<double> temp(0.0, 0.0);

    for(double l=0; l<point.size(); ++l)
    {
      std::complex<double> theta(2.0 * M_PI * l * frequency / point.size(), 0.0);
      temp += point.at(l) * exp(i * theta);
    }

    result.push_back(temp);
    frequency += frequency_steps;
  }

  return result;
}


void
DFT::normalize(std::vector<sf::Vector2f> *point, float y0, float normal_height)
{

  float c_pos = 0.0f;
  float s_pos = 0.0f;

  // Look for the highest point of both sine and cosine transform
  for(int i=0; i<point->size(); ++i)
  {
    float temp = y0 - point->at(i).y;
    if(i<point->size()/2 && s_pos < temp)
      s_pos = temp;
    else if(i >= point->size()/2 && c_pos > temp)
      c_pos = temp;
  }

  // Scale the height of the points to move the highest at the top of the window, then rescale the remaining points
  for(int i=0; i<point->size(); ++i)
  {
    if(i<point->size()/2)
      point->at(i).y = (y0 - point->at(i).y) / s_pos;
    else
      point->at(i).y = (y0 - point->at(i).y) / c_pos;

    point->at(i).y = y0 - (point->at(i).y * normal_height);
  }
}

//?___________________________________________________________________________________________________________
//? FFT

void
FFT::evaluate(std::vector<sf::Vector2f> *point, float y0)
{
  std::vector<std::complex<double>> complex_point;
  
  // Load the complex vector
  for(int i=0; i<point->size(); ++i)
  {
    std::complex<double> temp(point->at(i).y - y0, 0);
    complex_point.push_back(temp);
  }

  // If the input size isn't a power of 2, shrink the size to the closest power of two
  {
  int i;
  for(i = 0;; ++i)
    if(complex_point.size() < pow(2, i))
    {
      --i;
      break;
    }

  while(pow(2,i) < complex_point.size())
    complex_point.pop_back();
  }

  // Make sure that the output is also scaled
  while(complex_point.size() > point->size())
    point->pop_back();

  // Compute the FFT
  complex_point = transform(complex_point);

  // Load the output with the transform
  for(int i=0; i<complex_point.size(); ++i)
  { 
    point->at(i).y = y0 - sqrt(pow(complex_point.at(i).real(),2)
                     + pow(complex_point.at(i).imag(),2));
    point->at(i).x *= 2;
  }
}


std::vector<std::complex<double>> 
FFT::transform(std::vector<std::complex<double>> point)
{
  if(point.size() == 1)
    return point;

  std::complex<double> i(0.0,1.0);
  std::complex<double> w = exp(2.0 * i * M_PI / static_cast<double>(point.size()));
  
  std::vector<std::complex<double>> odd;
  std::vector<std::complex<double>> even;

  for(int k=0; k<point.size(); k+=2)
  {
    even.push_back(point.at(k));
    odd.push_back(point.at(k+1));
  }

  even = transform(even);
  odd = transform(odd);

  for(int k=0; k<point.size()/2; ++k)
  {
    point.at(k) = even.at(k) + std::pow(w, k) * odd.at(k);
    point.at(k + point.size()/2) = even.at(k) - std::pow(w, k) * odd.at(k); 
  }

  return point;
}