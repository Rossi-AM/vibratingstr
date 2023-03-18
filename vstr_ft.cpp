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

//?______________________________________________________________________________________________________________________________________________________________
//? DFT

void 
DFT::input(sf::Vector2f position, float time)
{
  // input thge position for the give time
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

  // if the given time doesn't exist create a new FT_DATA for such a time
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

  // search for the time 
  int i;
  for(i=0; i<time_data.size(); ++i)
    if(time == time_data.at(i).time) break;
  
  // if time is not found output the FFT at the last newly inputted time
  if(i == time_data.size())
    --i;

  result = time_data.at(i).position;

  // compute the DFT
  evaluate(&result);

  if(to_normalize)
    normalize(&result);

  return result;
}


std::vector<FT_DATA> 
DFT::evaluate_all_time(float y0, bool to_normalize, float normal_height)
{
  std::vector<FT_DATA> result;

  // evaluate for all times  
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
  // if input size is odd, makes it even;
  if(point->size()%2 == 1)
    point->pop_back();

  std::vector<std::complex<double>> complex_point;

  // load a complex vector with the input points
  for(int i = 0; i<point->size(); ++i)
  {
    std::complex<double> temp(point->at(i).y - y0, 0.0);
    complex_point.push_back(temp);
  }

  // compute the DFT
  complex_point =  transform(complex_point);

  // load the output with the transform
  // the first half of point is the fourier transform for a sine 
  // the second half of point is the fourier transform for a cosine
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
  // the sampling step garant that every point is a normal mode of the rope
  double frequency_steps = 0.5;

  std::complex i(0.0, 1.0);

  std::vector<std::complex<double>> result;

  // matrix vector moltipication of original function and test functions
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


// NOTE: this may cause trouble if used with FFT class. Please test this or let the authors know of possible problems
void
DFT::normalize(std::vector<sf::Vector2f> *point, float y0, float normal_height)
{

  float c_pos = 0.0f;
  float s_pos = 0.0f;

  // search for highest point of both sine and cosine transform
  for(int i=0; i<point->size(); ++i)
  {
    float temp = y0 - point->at(i).y;
    if(i<point->size()/2 && s_pos < temp)
      s_pos = temp;
    else if(i >= point->size()/2 && c_pos > temp)
      c_pos = temp;
  }

  // scale the height of the points such that the highest is the the top of the window and the other are on scale
  for(int i=0; i<point->size(); ++i)
  {
    if(i<point->size()/2)
      point->at(i).y = (y0 - point->at(i).y) / s_pos;
    else
      point->at(i).y = (y0 - point->at(i).y) / c_pos;

    point->at(i).y = y0 - (point->at(i).y * normal_height);
  }
}

//?______________________________________________________________________________________________________________________________________________________________
//? FFT

void
FFT::evaluate(std::vector<sf::Vector2f> *point, float y0)
{
  std::vector<std::complex<double>> complex_point;
  
  // load the complex vector
  for(int i=0; i<point->size(); ++i)
  {
    std::complex<double> temp(point->at(i).y - y0, 0);
    complex_point.push_back(temp);
  }

  // if the input size isn't a power of 2, shrink the size to the closest power of two
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

  // make sure that the output is also scaled
  while(complex_point.size() > point->size())
    point->pop_back();

  // comppute the FFT
  complex_point = transform(complex_point);

  //load the output with the transform
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