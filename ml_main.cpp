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

    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input, Constraint constraint/*, float radius, std::size_t pointCount*/);    
//    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input);
//    Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint);
//    Mass_Point(float mass_input, sf::Vector2f pos_input);
    
    void clear_acceleration() 
    void set_acceleration(sf::Vector2f acceleration);
    void get_position()
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
  public:
    Gravity(std::vector<Mass_Point> & mass_point_input)
    {
      for(auto i: mass_point)
        points->push_back(i);
    }

    void apply();

  private:
    const float g = 9.81;
    std::vector<Mass_Point> * points;
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
  int mass_point_num = 1000;
  int mass = 1;
  float k = 100;
  Gravity gravity(* mass_point);
  
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
  
  clock.restart();

  while(window.isOpen())
  {
    time = clock.getElapsedTime();

    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    gravity.apply()
    
    for(auto i: mass_point)
      i.update(time);

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

Mass_Point::set_acceleration(sf::Vector2f acceleration_input)
{
  acceleration += acceleration_input;
}

Mass_Point::get_mass()
{
  return mass;
}

Mass_point::update(sf::Time time)
{
  velocity.x += (acceleration.x * time.asMillisecond()); 
  velocity.y += (acceleration.y * time.asMillisecond()); 
  
  sf::vector2f new_position = point.getPosition();
  
  new_position.x += (velocity.x * time.asMillisecond());
  new_position.y += (velocity.y * time.asMillisecond());

  point.setPosition(new_position);

  clear_acceleration();
}

//?______________________________________________________________________________________________________________________________________________________________
//? gravity

void
Gravity::apply()
{
  for(auto i: mass_point)
  {
    sf::Vector2f acceleration_input(0.0 , g);
    i->set_acceleration(acceleration_input);
  }
}

//?______________________________________________________________________________________________________________________________________________________________
//? spring

Spring::Spring(int k_input)
{
  k = k_input;
  rest_lenght = window.getSize().x/(mass_point_num - 1)
}

git add .
git commit -m "messaggio"
git push