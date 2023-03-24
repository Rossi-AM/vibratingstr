
//                                     
// Rossi A. M. - Tambini M.    03/2023 
// 

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

//!______________________________________________________________________________________________________________________________________________________________
//! class method


//? ________________________________________________________________________________________________________
//? Linear_Shape

Linear_Shape::Linear_Shape(std::string function_name,
                           float amplitude, 
                           float width, 
                           float repetitions)
{
  builder(function_name, amplitude, width, repetitions);
}

Linear_Shape::Linear_Shape(float amplitude,
             float width,
             float repetitions)
{
  builder(DEFAULT_CURRENT_FUNCTION, amplitude, width, repetitions);
}

void
Linear_Shape::builder(std::string function_name, 
                      float amplitude,
                      float width,
                      float repetitions)
{
  this->current_function = function_name;
  this->data.amplitude = amplitude;
  this->data.width = width;
  this->data.repetitions = repetitions;

  // Load all initialization functions included in the library
  function_loader();  
}

//* public

void
Linear_Shape::set_amplitude(float amplitude)
{
  if(amplitude > 1)
    amplitude = 1;
  else if(amplitude < 0)
    amplitude = 0;

  this->data.amplitude = amplitude;
}

void 
Linear_Shape::set_repetitions(float repetitions) {this->data.repetitions = repetitions;}

void
Linear_Shape::set_width(float width)
{
  if(width > 1)
    width = 1;
  else if(width < 0)
    width = 0;

  this->data.width = width;
}


void
Linear_Shape::add_function(std::string name, linear_method foo)
{
  // Create a new element of map functions with key name and function foo
  functions[name] = [this, foo](std::vector<sf::Vector2f>* position, float length, float height)
  { foo(position, length, height, data); };
}

void 
Linear_Shape::change_function(std::string name) {this->current_function = name;}


void
Linear_Shape::apply(std::vector<sf::Vector2f>* position, float length, float height)
{
  // call current_function passing input variables
  functions[current_function](position, length, height);
}

//* private

void
Linear_Shape::function_loader()
{
  // Distribute positions as an horizontal line 
  add_function("line",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(int i = 0; i < position->size(); ++i)
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };
  });

  // Distribute positions as a sine, if width < 1 the remaining length is initialized as a line
  add_function("sine",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(float i = 1.0f; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
        position->at(i).y = position->at(0).y - std::sin( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) * height * (LENGHT_UNIT * data.amplitude / 2.0f);
    }
  });

  //  Distribute positions as a cosine, if width < 1 the remaining length is initialized as a line
  add_function("cosine",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    sf::Vector2f pos0 = position->at(0);

    for(float i = 0; i < position->size(); ++i)
    {
      position->at(i) = { pos0.x + i * length * (LENGHT_UNIT / position->size()), pos0.y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
        position->at(i).y = pos0.y - std::cos( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) * height * (LENGHT_UNIT * data.amplitude / 2.0f);
    }
  });

  //  Distribute positions as a square-wave sine, if width < 1 the remaining length is initialized as a line
  add_function("square",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(int i = 1; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {

        if(std::sin(static_cast<float>(i) / position->size() / data.width * data.repetitions * 2.0f * M_PI) < 0)
          position->at(i).y = position->at(0).y - (height * LENGHT_UNIT * data.amplitude / 2.0f);
        else
          position->at(i).y = position->at(0).y + (height * LENGHT_UNIT * data.amplitude / 2.0f);

      }

      position->at(position->size()-1).y = position->at(0).y;
    }
  });

  //  Distribute positions as a triangular-wave sine, if width < 1 the remaining length is initialized as a line
  add_function("triangle",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(float i = 1.0f; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {
        float height_increment = height * (LENGHT_UNIT / position->size()) * data.amplitude / data.width * 2.0f * data.repetitions;

        if(std::cos( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) > 0)
          position->at(i).y = position->at(i-1).y + height_increment;
        else  
          position->at(i).y = position->at(i-1).y - height_increment;
      }
    }
  });

  //  Distribute positions as a pulse
  add_function("wave",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    sf::Vector2f pos0 = position->at(0);

    for(float i = 0.0f; i < position->size(); ++i)
    {
      position->at(i) = { pos0.x + i * length * (LENGHT_UNIT / position->size()), pos0.y };

      if(pos0.x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {
        position->at(i).y = pos0.y - (1 - std::cos( i / position->size() / data.width * data.repetitions * M_PI + M_PI)) * height * (LENGHT_UNIT * data.amplitude / 2);
      }
    }
  });
}

//? ________________________________________________________________________________________________________
//? Rope

Rope::Rope(unsigned int point_number, 
           float mass, 
           float tension, 
           float length, 
           sf::Vector2f position , 
           Constraint a, 
           Constraint b, 
           sf::Color color)
{
  builder(point_number, mass, 
          tension, length, position, 
          a, 
          b, 
          color);
}

Rope::Rope(unsigned int point_number, 
           float mass, 
           float tension, 
           float length, 
           sf::Vector2f position, 
           sf::Color color)
{
  builder(point_number, mass, tension, 
          length, position,
           a, 
           b, 
           color);
}

Rope::Rope(Rope* left, Rope* right)
{
  // Keep the constraint of the extreme 
  this->a = left->get_mass(0).get_constraint();
  this->b = right->get_mass(right->size() -1).get_constraint();

  // Add all the masses of the left rope and set their constraint to zero
  for(int i = 0; i < left->size(); ++i)
  {
    this->mass_point.push_back(left->get_mass(i));
    this->mass_point.back().set_constraint();
  }

  // Add all the masses of the right rope and set their constraint to zero
  for(int i = 0; i < right->size(); ++i)
  {
    this->mass_point.push_back(right->get_mass(i));
    this->mass_point.back().set_constraint();
  }

  // Add all the springs of the left rope
  for(int i=0; i<left->size()-1; ++i)
  {
    Spring temp(left->get_spring(i).get_k(), 
                &mass_point.at(i), 
                &mass_point.at(i+1));

    this->spring.push_back(temp);
  }

  // Add a single spring to connect left and right ropes
  {
    Spring temp(0, 
                &mass_point.at(left->size()-1), 
                &mass_point.at(left->size()));
    
    temp.set_k((left->get_tension() 
               + right->get_tension()) 
               / 2.0f / temp.get_length());

    this->spring.push_back(temp);
  }

  // Add all the springs of the right rope
  for(int i=0; i<right->size()-1; ++i)
  {
    Spring temp(right->get_spring(i).get_k(), 
                &mass_point.at(left->size() + i), 
                &mass_point.at(left->size() + i+1));

    this->spring.push_back(temp);
  }

  // Initialize all the other variables
  this->point_number = mass_point.size();
  this->mass = left->get_mass() + right->get_mass();
  update_tension();
  update_length();
  this->position = mass_point.at(0).get_position();
  this->color = DEFAULT_COLOR;             // color isn't updated until set_color is called 

  // Set extreme constraints based on left and right ropes constraints
  set_constraint(0, a);
  set_constraint(point_number - 1, b);
}

void 
Rope::builder(unsigned int point_number, float mass, float tension, 
              float length, sf::Vector2f position,  
              Constraint a, Constraint b, sf::Color color)
{
  this->point_number = point_number;
  this->mass = mass;
  this->length = length * LENGHT_UNIT;
  this->tension = tension;
  this->length = length;
  this->position = position;
  this->a = a;
  this->b = b;
  this->color = color;

  // Construct all masses
  this->mass_point = mass_builder();

  // Construct all springs
  this->spring = spring_builder();
}

std::vector<Mass_Point> 
Rope::mass_builder()
{
  std::vector<Mass_Point> mass_point;

  for(int i = 0; i < point_number; ++i)
  {
    sf::Vector2f temp_pos(position.x + i * length / point_number, position.y);
    
    Mass_Point temp(mass / point_number, temp_pos, color);
 
    mass_point.push_back(temp);
  }

  mass_point.front().set_constraint(a);
  mass_point.back().set_constraint(b);

  return mass_point;
}

std::vector<Spring>
Rope::spring_builder()
{
  std::vector<Spring> spring;
  float k = (tension / length) * point_number;

  for(int i = 0; i < mass_point.size() - 1 ; ++i)
  {
    Spring temp(k, &mass_point.at(i), &mass_point.at(i+1));

    spring.push_back(temp);
  }

  return spring;
}

//* public

float 
Rope::get_tension() {update_tension(); return tension;}

float 
Rope::get_length() {update_length(); return length;}

float 
Rope::get_mass() {return mass;}

unsigned int 
Rope::size() {return point_number;}

void
Rope::set_constraint(unsigned int i, bool x, bool y)
{
  Constraint constraint(x, y);
  set_constraint(i, constraint);
}

void
Rope::set_constraint(unsigned int i, Constraint constraint)
{
  if(i >= point_number)
    i = point_number-1;

  this->mass_point.at(i).set_constraint(constraint);
}

void
Rope::set_color(sf::Color color)
{
  for(int i = 0; i < mass_point.size(); ++i)
    mass_point.at(i).set_color(color);
}

void
Rope::set_shape(Linear_Shape shape, float oscillation_amplitude)
{
  std::vector<sf::Vector2f> new_position;

  for(int i = 0; i < mass_point.size(); ++i)
    new_position.push_back(mass_point.at(i).get_position());

  shape.apply(&new_position, length,
              oscillation_amplitude);

  for(int i = 0; i < new_position.size(); ++i)
    mass_point.at(i).set_position(new_position.at(i));
}

Mass_Point
Rope::get_mass(unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;  

  return this->mass_point.at(i); 
};


Spring
Rope::get_spring(unsigned int i)
{  
  if(i >= point_number)
    i = point_number -1;

   return this->spring.at(i); 
};

sf::Vector2f
Rope::get_position_at(unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;


  return this->mass_point.at(i).get_position(); 
};

sf::Vector2f
Rope::get_velocity_at(unsigned int i)
{ 
  if(i >= point_number)
    i = point_number -1;

  return this->mass_point.at(i).get_velocity(); 
};


Spring
Rope::attach_mass(Mass_Point* mass, float k, unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;

  Spring new_spring(k, mass, &mass_point.at(i));

  return new_spring;
}

void
Rope::set_x_sliding(bool x)
{
  for(int i = 1; i < mass_point.size() - 1; ++i)
    mass_point.at(i).set_constraint(!x, mass_point.at(i).get_constraint().y);
}

// Pass all masses of rope to gravity
void 
Rope::add_gravity(Gravity* gravity) {gravity->add(&mass_point);} 

void
Rope::apply()
{
  for(int i = 0; i < spring.size(); ++i)
    spring.at(i).apply();
}

void
Rope::update(float time_increment)
{
  for(int i = 0; i < mass_point.size(); ++i)
    mass_point.at(i).update(time_increment);

  this->position = get_position_at(0);
}

//* private

void
Rope::update_tension()
{
  float force = 0.0f;

  for(int i = 0; i < spring.size(); ++i)
    force  = force * (i / (i+1)) 
           + spring.at(i).get_k() * 
             spring.at(i).get_length() / 
             (i+1);   

  this->tension = force * spring.size();
}

void
Rope::update_length()
{
  this->length = 0;

  for(auto i: spring)
    this->length += i.get_length();
}