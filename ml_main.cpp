#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Classes

class Constraint
{
  public:
    Constraint () {x = false; y = false;};
    Constraint (bool x_input, bool y_input)
      {
        x = x_input;
        y = y_input;
      }

    //~Constraint();


    bool x, y;
};

class Mass_Point
{ 
  public:

    Mass_Point() {};
    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input, Constraint constraint/*, float radius, std::size_t pointCount*/);    
//    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input);
//    Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint);
//    Mass_Point(float mass_input, sf::Vector2f pos_input);



    void clear_acceleration();
    void set_acceleration(sf::Vector2f acceleration);
    void update(sf::Time time);
    float get_mass();
    
    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;
    
};


class Spring
{
  public:

    Spring(int k_input);
 

    void apply();

  private: 
    
    void update_lenght();
    float k;
    float lenght;
    float rest_lenght;
    Mass_Point *m1, *m2;
};


class Gravity
{
  private:
    const float g = 9.81 / 100000000000;
    std::vector<Mass_Point*> points;

  public:
    Gravity(std::vector<Mass_Point> *mass_point_input)
    {
      for(int i=0; i<mass_point_input->size(); ++i)
      {
        Mass_Point *temp;
        temp = &mass_point_input->at(i);
        points.push_back(temp);
      }
    }

    void apply();
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

  sf::RenderWindow window(sf::VideoMode(1000, 600), "use this to hang up");

  std::vector<Mass_Point> mass_point;
  std::vector<Spring> spring;
  int mass_point_num = 10;
  int mass = 1;
  float k = 100;

  
  sf::Time time;
  sf::Clock clock;
  
  sf::Vector2f vel_input(0.0, 0.0);
  Constraint constraint_input(false, false);
  
  for(int i = 0; i < mass_point_num; ++i)
  {
    sf::Vector2f pos_input((window.getSize().x/(mass_point_num - 1))*i, window.getSize().y/2);

    Mass_Point temp(mass, pos_input, vel_input, constraint_input);

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

    int mestruo = 0;
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
//      std::cout << "Ciclo " << mestruo << " Punto " << i << " " << mass_point.at(i).point.getPosition().x << " " << mass_point.at(i).point.getPosition().y << "\n";
    }

     mestruo++;
    
    window.clear();
    for(auto i: mass_point)
      window.draw(i.point);
    window.display();
  }
  std::cout << "Hello Macchia!\n";
  return 0;
}





//!______________________________________________________________________________________________________________________________________________________________
//! class method

//?______________________________________________________________________________________________________________________________________________________________
//? mass point

//ufficiale
Mass_Point::Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input, Constraint constraint_input/*, float radius, std::size_t pointCount*/)
{  
  point.setRadius(3.0/*radius*/);
  point.setPointCount(30/*pointCount*/);
  point.setOrigin(1.5, 1.5);
  point.setPosition(pos_input);

  mass = mass_input;
  velocity = velocity_input;
  acceleration.x = 0;
  acceleration.y = 0;
  constraint = constraint_input;
}   

/*
Mass_Point::Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input)
 {
   Constraint constraint_input(false, false);
   Mass_Point (mass_input, pos_input, velocity_input, constraint_input);
 }

Mass_Point::Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint)
 {
   sf::Vector2f velocity_input(0.0,0.0);
   Mass_Point(mass_input, pos_input, velocity_input, constraint);
 }

    
Mass_Point::Mass_Point(float mass_input, sf::Vector2f pos_input)
 {
   Constraint constraint_input(false, false);
   sf::Vector2f velocity_input(0.0, 0.0);
   Mass_Point(mass_input, pos_input, velocity_input, constraint_input);
 }
*/

void
Mass_Point::clear_acceleration()
{
  acceleration.x = 0.0;
  acceleration.y = 0.0;
}

void
Mass_Point::set_acceleration(sf::Vector2f acceleration_input)
{
  acceleration.x += acceleration_input.x;
  acceleration.y += acceleration_input.y;
}

float
Mass_Point::get_mass()
{
  return mass;
}

void
Mass_Point::update(sf::Time time)
{

  sf::Int64 msec = time.asMicroseconds();

  velocity.x += (acceleration.x * time.asMicroseconds()); 
  velocity.y += (acceleration.y * time.asMicroseconds()); 

  sf::Vector2f new_position = point.getPosition();
  
  new_position.x += (velocity.x * time.asMicroseconds());
  new_position.y += (velocity.y * time.asMicroseconds());

//  std::cout << std::endl << "DEBUG" << std::endl << new_position.y << ' ' << msec << std::endl;

  point.setPosition(new_position);

  clear_acceleration();
}

//?______________________________________________________________________________________________________________________________________________________________
//? gravity

void
Gravity::apply()
{
  for(int i=0; i<points.size(); ++i)
  {
    sf::Vector2f acceleration_input(0.0 , g);
    points.at(i)->set_acceleration(acceleration_input);
  }
}

//?______________________________________________________________________________________________________________________________________________________________
//? spring
/*
Spring::Spring(int k_input)
{
  k = k_input;
  rest_lenght = window.getSize().x/(mass_point_num - 1)
}
*/