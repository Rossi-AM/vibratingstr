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
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_basic.h"

//!______________________________________________________________________________________________________________________________________________________________
//! Class method

//?______________________________________________________________________________________________________________________________________________________________
//? Constraint

Constraint::Constraint()
{
  this->x = false;
  this->y = false;
}

Constraint::Constraint(bool x, bool y)
{
  this->x = x;
  this->y = y;
}

//?______________________________________________________________________________________________________________________________________________________________
//? Mass point 

Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Vector2f velocity, 
                       Constraint constraint, 
                       sf::Color color)
{  
  builder(mass, position, velocity, constraint, color);
}   


Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       Constraint constraint, 
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}

Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Vector2f velocity,
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}

    
Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}



void
Mass_Point::builder(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint, sf::Color color)
{
  // Initialization of the SFML object CircleShape
  point.setRadius(DEFAULT_P_RADIUS);
  point.setPointCount(DEFAULT_C_POINT_COUNT);
  point.setOrigin(DEFAULT_P_RADIUS / 2 , DEFAULT_P_RADIUS / 2);
  point.setPosition(position);
  point.setFillColor(color);

  // Initialization of class variables
  this->mass = mass;
  this->velocity = velocity;
  this->constraint = constraint;
  // Setting initial acceleration to 0
  clear_acceleration();
}

//* public

void
Mass_Point::clear_acceleration()
{
  acceleration.x = 0.0;
  acceleration.y = 0.0;
}

void
Mass_Point::update_acceleration(sf::Vector2f acceleration)
{
  this->acceleration.x += acceleration.x;
  this->acceleration.y += acceleration.y;
}

float
Mass_Point::get_mass() {return mass;}

sf::Vector2f
Mass_Point::get_velocity() {return velocity;}

void
Mass_Point::set_velocity(sf::Vector2f velocity) {this->velocity = velocity;}

void
Mass_Point::set_velocity(float x, float y)
{
  this->velocity.x = x;
  this->velocity.y = y;
};

sf::Vector2f
Mass_Point::get_position() {return point.getPosition();}

void 
Mass_Point::set_position(sf::Vector2f position) {this->point.setPosition(position);}

void 
Mass_Point::set_position(float x, float y) { this->point.setPosition(x,y); }

void 
Mass_Point::set_constraint(Constraint constraint) {this->constraint = constraint;}

void 
Mass_Point::set_constraint(bool x, bool y)
{
  this->constraint.x = x; 
  this->constraint.y = y;
};

Constraint 
Mass_Point::get_constraint() {return constraint;}

void 
Mass_Point::set_color(sf::Color color) {this->point.setFillColor(color);}

void
Mass_Point::update(float time_increment)
{ 
  // Updating x axis velocity while checking for constraints
  if(constraint.x)
    velocity.x = 0.0;
  else
    velocity.x += (acceleration.x * time_increment); 
  
  // Updating y axis velocity while checking for constraints
  if(constraint.y)
    velocity.y = 0.0;
  else
    velocity.y += (acceleration.y * time_increment); 

  // Updating position
  sf::Vector2f new_position = point.getPosition();
  
  new_position.x += velocity.x * (time_increment * LENGHT_UNIT);
  new_position.y += velocity.y * (time_increment * LENGHT_UNIT);

  point.setPosition(new_position);
  
  // Resetting acceleration for the new time increment
  clear_acceleration();
}


//?______________________________________________________________________________________________________________________________________________________________
//? Gravity

Gravity::Gravity(std::vector<Mass_Point>* mass_point)
{
  this->g.x = 0.0;
  this->g.y = 9.80665;
  add(mass_point);
}

Gravity::Gravity()
{
  this->g.x = 0.0;
  this->g.y = 9.80665;
}

//* public

void
Gravity::add(std::vector<Mass_Point>* mass_point)
{
  for(int i=0; i<mass_point->size(); ++i)
  {
    Mass_Point* temp;
    temp = &mass_point->at(i);
    points.push_back(temp);    
  }
}

void
Gravity::add(Mass_Point* mass_point)
{
  points.push_back(mass_point);
}

void
Gravity::apply()
{
  for(int i=0; i<points.size(); ++i)
    points.at(i)->update_acceleration(g);
}

//?______________________________________________________________________________________________________________________________________________________________
//? Spring

Spring::Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_length) 
{builder(k, m1, m2, rest_length = DEFAULT_REST_L);}

Spring::Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest) 
{builder(k, m1, m2, get_length(m1, m2));}

void
Spring::builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_length)
{
  this->k = k;
  this->m1 = m1;
  this->m2 = m2;
  set_rest_length(rest_length);
}

//* public

void 
Spring::set_rest_length(float rest_length) {this->rest_length = rest_length;}

void 
Spring::set_k(float k) {this->k = k;}
    
float 
Spring::get_k() {return k;}

float 
Spring::get_length() {update_length(); return length;}

void
Spring::apply()
{
  sf::Vector2f force;
  sf::Vector2f a1, a2;

  // Updating length before force computation 
  update_length();
  update_vector_length();

  // Computing force on x and y axis
  force.x = (length - rest_length) * k * (vector_length.x / length);
  force.y = (length - rest_length) * k * (vector_length.y / length);

  // Computing new acceleration
  a1 = (force / m1->get_mass());
  a2 = ((-force) / m2->get_mass());

  // Checking the acceleration sign on x and y axys
  if(m1->get_position().x > m2->get_position().x)
  {
    a1.x *= (-1); 
    a2.x *= (-1); 
  }

  if(m1->get_position().y > m2->get_position().y)
  {
    a1.y *= (-1); 
    a2.y *= (-1); 
  } 

  // Updating the acceleration of the two masses
  m1->update_acceleration(a1);
  m2->update_acceleration(a2);
}

//* private

void
Spring::update_vector_length() { update_vector_length(m1, m2); }

void
Spring::update_vector_length(Mass_Point* m1, Mass_Point* m2)
{
  this->vector_length.x = abs(m1->get_position().x * 1000 
                            - m2->get_position().x * 1000) 
                            / (LENGHT_UNIT * 1000);
  this->vector_length.y = abs(m1->get_position().y * 1000 
                            - m2->get_position().y * 1000) 
                            / (LENGHT_UNIT * 1000);
}

void
Spring::update_length() {update_length(m1, m2);}

void
Spring::update_length(Mass_Point* m1, Mass_Point* m2)
{
  this->length = sqrt( pow((m1->get_position().x * 1000 
                     - m2->get_position().x * 1000) / (LENGHT_UNIT * 1000), 2) 
                     + pow((m1->get_position().y * 1000 
                     - m2->get_position().y * 1000) / (LENGHT_UNIT * 1000), 2));
}

float 
Spring::get_length(Mass_Point* m1, Mass_Point* m2) 
{
  update_length(m1, m2); 
  return length;
}