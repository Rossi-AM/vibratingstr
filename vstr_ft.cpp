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
DFT::evaluate_at_time(float time)
{
  std::vector<sf::Vector2f> result;

  int i;
  for(i=0; i<time_data.size(); ++i)
    if(time == time_data.at(i).time) break;
  
  if(i == time_data.size())
    --i;

  result = evaluate(time_data.at(i).position);

  return result;  
}


std::vector<FT_DATA> 
DFT::evaluate_all_time()
{
  std::vector<FT_DATA> result;
  
  for(int i=0; i<time_data.size(); ++i)
  {
    FT_DATA temp;
    temp.position = evaluate_at_time(time_data.at(i).time);
    temp.time = time_data.at(i).time;
    result.push_back(temp);
  }

  return result;
}


std::vector<sf::Vector2f> 
DFT::evaluate(std::vector<sf::Vector2f> point)
{
  if(point.size()%2 == 1)
    point.pop_back();

  float y0 = point.front().y;
  std::vector<std::complex<double>> complex_point;

  for(int i = 0; i<point.size(); ++i)
  {
    std::complex<double> temp(point.at(i).y - y0, 0.0);
    complex_point.push_back(temp);
  }

  complex_point = transform(complex_point);

  for(int i=0; i<complex_point.size()/2; ++i)
  {
    point.at(i).x *= 2.0;
    point.at(i).y = y0 - abs(complex_point.at(i).imag());

    point.at(i+complex_point.size()/2).x = point.at(i).x;
    point.at(i+complex_point.size()/2).y = y0 - abs(complex_point.at(i).real());
  }

  return point;
}


std::vector<std::complex<double>> 
DFT::transform(std::vector<std::complex<double>> point)
{
  double frequency_steps = 0.5;

  std::complex i(0.0, 1.0);

  std::vector<std::complex<double>> result;

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

//?______________________________________________________________________________________________________________________________________________________________
//? FFT

std::vector<sf::Vector2f> 
FFT::evaluate(std::vector<sf::Vector2f> point)
{
  float y0 = point.front().y;

  std::vector<std::complex<double>> complex_point;
  for(int i=0; i<point.size(); ++i)
  {
    std::complex<double> temp(point.at(i).y - y0, 0);
    complex_point.push_back(temp);
  }

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

  complex_point = transform(complex_point);

  std::vector<sf::Vector2f> result;

  for(int i=0; i<complex_point.size(); ++i)
  { 
    sf::Vector2f temp(0.0f,0.0f);
    temp.y = y0 - sqrt(pow(complex_point.at(i).real(),2)
                     + pow(complex_point.at(i).imag(),2));
    temp.x = point.at(i).x;
    result.push_back(temp);
  }

  for(int i=0; i<complex_point.size()/2; ++i)
  {
    point.pop_back();
    point.at(i).x *= 2.0;
  }

  return result;
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