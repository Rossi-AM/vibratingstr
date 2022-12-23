#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define DEFAULT_VEL sf::Vector2f(0.0f, 0.0f)
#define DEFAULT_CONSTRAINT Constraint()

// Classes

class Constraint
{
  public:
    Constraint() { x = false; y = false; };
    Constraint(bool x, bool y)
      {
        this->x = x;
        this->y = y;
      }

    //~Constraint();

    bool x, y;
};

class Mass_Point
{ 
  public:

    Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint/*, float radius, std::size_t pointCount*/);    
    Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity);
    Mass_Point(float mass, sf::Vector2f position, Constraint constraint);
    Mass_Point(float mass, sf::Vector2f position);



    void clear_acceleration();
    void set_acceleration(sf::Vector2f acceleration);
    void update(sf::Time time);
    float get_mass() { return mass; };
    sf::Vector2f get_position() { return point.getPosition(); };
    
    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;
    
    void builder(float mass, sf::Vector2f pos, sf::Vector2f velocity, Constraint constraint);
};


class Gravity
{
  public:

    Gravity(std::vector<Mass_Point> *mass_point);


    void apply();
  

  private:

    const float g = 9.81 / 100000000000;
    std::vector<Mass_Point*> points;

};


class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2) 
      { builder(k, m1, m2, 0); }
    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_lenght) 
      { builder(k, m1, m2, rest_lenght); }
    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest) 
      { builder(k, m1, m2, lenght(m1, m2)); }


    void apply();
    void set_rest_lenght(float rest_lenght) { this->rest_lenght = rest_lenght;};


  private: 
    
    float k;
    float rest_lenght;
    Mass_Point *m1, *m2;

    void builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_lenght);

    float lenght();
    float lenght(Mass_Point* m1, Mass_Point* m2);
    sf::Vector2f vector_lenght();                                                   // output the distance in sf::Vector2f between the extremis of the spring
    sf::Vector2f vector_lenght(Mass_Point* m1, Mass_Point* m2);                     // output the distance in sf::Vector2f between the arguments
};

/*
class rope ()
{
  

  private:
    float head_pos;
    float tail_pos;
    float lenght = std::abs(head_pos - tail_pos);
    int point_number;
    float total_mass;

}
*/

//!________________________________________________________________________________________________________
//! MAIN

int main(int argc, char const *argv[])
{

  sf::RenderWindow window(sf::VideoMode(1000, 600), "when will ale-senpai notice me");

  std::vector<Mass_Point> mass_point;
  int mass_point_num = 10;
  int mass = 1;
  float k = 100;
  
  sf::Time time;
  sf::Clock clock;
  
  for(int i = 0; i < mass_point_num; ++i)
  {
    sf::Vector2f position((window.getSize().x/(mass_point_num - 1))*i, window.getSize().y/2);

    Mass_Point temp(mass, position);

    mass_point.push_back(temp);
    /*
    if(i==0) continue;
    else
    {
      spring.push_back(k);
    }
    
    a che cazzo serve questo ale? e non mi dire che hai scritto solo questo in mezzora e passa...
    */
  }
  
  Gravity gravity(&mass_point);
  clock.restart();

  while(window.isOpen())
  {
    time = clock.getElapsedTime();
    clock.restart();

    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    gravity.apply();
    
    
    for(int i = 0; i < mass_point.size(); ++i)
    {
      mass_point.at(i).update(time);
    }
    
    window.clear();
    for(auto i: mass_point)
      window.draw(i.point);
    window.display();
  }
  return 0;
}





//!______________________________________________________________________________________________________________________________________________________________
//! class method

//?______________________________________________________________________________________________________________________________________________________________
//? mass point

Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint/*, float radius, std::size_t pointCount*/)
{  
  builder(mass, position, velocity, constraint);
}   


Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity)
 {
   Constraint constraint(false, false);
   builder(mass, position, velocity, constraint);
 }

Mass_Point::Mass_Point(float mass, sf::Vector2f position, Constraint constraint)
 {
   sf::Vector2f velocity(0.0, 0.0);
   builder(mass, position, velocity, constraint);
 }

    
Mass_Point::Mass_Point(float mass, sf::Vector2f position)
 {
   Constraint constraint(false, false);
   sf::Vector2f velocity(0.0, 0.0);
   builder(mass, position, velocity, constraint);
 }

void
Mass_Point::builder(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint)
{
  point.setRadius(3.0/*radius*/);
  point.setPointCount(30/*pointCount*/);
  point.setOrigin(1.5, 1.5);
  point.setPosition(position);

  this->mass = mass;
  this->velocity = velocity;
  clear_acceleration();
  this->constraint = constraint;
}

void
Mass_Point::clear_acceleration()
{
  acceleration.x = 0.0;
  acceleration.y = 0.0;
}

void
Mass_Point::set_acceleration(sf::Vector2f acceleration)
{
  this->acceleration.x += acceleration.x;
  this->acceleration.y += acceleration.y;
}

void
Mass_Point::update(sf::Time time)
{
  if(constraint.x)
    velocity.x = 0.0;
  else
    velocity.x += (acceleration.x * time.asMicroseconds()); 
  
  if(constraint.y)
    velocity.y = 0.0;
  else
    velocity.y += (acceleration.y * time.asMicroseconds()); 
  

  
  sf::Vector2f new_position = point.getPosition();
  
  new_position.x += (velocity.x * time.asMicroseconds());
  new_position.y += (velocity.y * time.asMicroseconds());

  point.setPosition(new_position);

  clear_acceleration();
}

//?______________________________________________________________________________________________________________________________________________________________
//? gravity

Gravity::Gravity(std::vector<Mass_Point> *mass_point)
{
  for(int i=0; i<mass_point->size(); ++i)
  {
    Mass_Point *temp;
    temp = &mass_point->at(i);
    points.push_back(temp);
    
  }
}

void
Gravity::apply()
{
  for(int i=0; i<points.size(); ++i)
  {
    sf::Vector2f acceleration(0.0 , g);
    points.at(i)->set_acceleration(acceleration);
  }
}

//?______________________________________________________________________________________________________________________________________________________________
//? spring

void
Spring::builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_lenght)
{
  this->k = k;
  this->m1 = m1;
  this->m2 = m2;
  set_rest_lenght(rest_lenght);
}

sf::Vector2f
Spring::vector_lenght() { return vector_lenght(m1, m2); }

sf::Vector2f
Spring::vector_lenght(Mass_Point* m1, Mass_Point* m2)
{
  sf::Vector2f lenght;
  lenght.x = sqrt( pow( (m1->get_position().x - m2->get_position().x) , 2) );
  lenght.y = sqrt( pow( (m1->get_position().y - m2->get_position().y) , 2) );

  return lenght;
}

void
Spring::apply()
{
  sf::Vector2f force;

}