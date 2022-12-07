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
    // Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input);
    // Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint);
    // Mass_Point(float mass_input, sf::Vector2f pos_input);
    
    //~Mass_Point();
    
    void compute_acceleration(sf::Vector2f force);
    void update();
    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;
    
};



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


// Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input)
// {
//   Constraint constraint_input(false, false);
//   Mass_Point(mass_input, pos_input, velocity_input, constraint_input);
// }


// Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint)
// {
//   sf::Vector2f velocity_input(0.0,0.0);
//   Mass_Point(mass_input, pos_input, velocity_input, constraint);
// }

    
// Mass_Point(float mass_input, sf::Vector2f pos_input)
// {
//   Constraint constraint_input(false, false)
//   sf::Vector2f velocity_input(0.0, 0.0);
//   Mass_Point(mass_input, pos_input, velocity_input, constraint_input);
// }


/*
class Spring ()
{
  public:
    void apply(Mass_Point &m1, Mass_Point &m2);
  private: 
    
    void update_lenght();
    float k;
    float lenght;
    float rest_lenght;
    Mass_Point *m1, *m2;

    

    float apply_force(Mass_Point m);

};

Spring::apply(Mass_Point &m1, Mass_Point &m2)
{

}

class Gravity ()
{
  public:
    void apply(Mass_point &m)
  private:
    float g = 9.81;
};
*/

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

int main(int argc, char const *argv[])
{

  sf::RenderWindow window(sf::VideoMode(1000, 600), "use this to hang up");

  std::vector<Mass_Point> mass_point;
  int mass_point_num = 1000;
  int mass = 1;

  
  for(int i = 0; i < mass_point_num; ++i)
  {
    sf::Vector2f pos_input((window.getSize().x/(mass_point_num - 1))*i, window.getSize().y/2);
    sf::Vector2f vel_input(0.0, 0.0);
    Constraint constraint_input(false, false);

    Mass_Point temp(mass, pos_input, vel_input, constraint_input);

    mass_point.push_back(temp);
  }
  
  while(window.isOpen())
  {
    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    window.clear();
    
    for(auto i: mass_point)
      window.draw(i.point);

    window.display();
  }
  std::cout << "Hello Macchia!\n";
  return 0;
}


















































