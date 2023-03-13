#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_fft.h"

//?______________________________________________________________________________________________________________________________________________________________
//? FFT


void 
FFT::input(sf::Vector2f position, float time, unsigned int index)
{
  input_by_time(position, time);

  bool check = true;
  for(int i = 0; i<index_data.size(); ++i)
  {
    if(index == index_data.at(i).index)
    {
      index_data.at(i).position.push_back(position);
      check = false;
      break;
    }
  }

  if(check)
  {
    FFT_Data temp;
    temp.position.push_back(position);
    temp.index = index;
    time_data.push_back(temp);
  }
}


void 
FFT::input_by_time(sf::Vector2f position, float time)
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
    FFT_Data temp;
    temp.position.push_back(position);
    temp.time = time;
    time_data.push_back(temp);
  }
}


void 
FFT::input_by_time(std::vector<sf::Vector2f> position, float time)
{
  for(int i=0; i<position.size(); ++i)
    input_by_time(position.at(i), time);
}


std::vector<sf::Vector2f>
FFT::evaluate_at_time(float time)
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

std::vector<sf::Vector2f> 
FFT::evaluate_for_index(unsigned int index)
{
  std::vector<sf::Vector2f> result;
  
  int i;
  for(i=0; i<index_data.size(); ++i)
    if(index == index_data.at(i).index) break;

  if(i == index_data.size())
    --i;

  result = evaluate(index_data.at(i).position);

  return result;
}


std::vector<FFT_Data> 
FFT::evaluate_all_time()
{
  std::vector<FFT_Data> result;
  
  for(int i=0; i<time_data.size(); ++i)
  {
    FFT_Data temp;
    temp.position = evaluate_at_time(time_data.at(i).time);
    temp.time = time_data.at(i).time;
    result.push_back(temp);
  }

  return result;
}


std::vector<sf::Vector2f> 
FFT::evaluate(std::vector<sf::Vector2f> point)
{
  float y0 =point.front().y;

  std::vector<std::complex<double>> compelx_point;
  for(int i=0; i<point.size(); ++i)
  {
    std::complex<double> temp(point.at(i).y - y0, 0);
    compelx_point.push_back(temp);
  }

  {
  int i;
  for(i = 0;; ++i)
    if(compelx_point.size() < pow(2, i))
    {
      --i;
      break;
    }

  while(pow(2,i) < compelx_point.size())
    compelx_point.pop_back();
  }

  compelx_point = transform(compelx_point);

  std::vector<sf::Vector2f> result;

  for(int i=0; i<point.size(); ++i)
  { 
    sf::Vector2f temp(0.0f,0.0f);
    temp.y = y0 - sqrt(pow(compelx_point.at(i).real(),2)
                     + pow(compelx_point.at(i).imag(),2));
    temp.x = point.at(i).x;
    result.push_back(temp);
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