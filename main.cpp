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
  Constraint () {x = false; y = false;};
  Constraint (x_input, y_input)
    {
      x = x_input;
      y = y_input;
    }

  ~Constraint();

  public:

    bool x, y;
};

class Mass_Point
{ 
  public:

    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input, Constraint constraint);    
    Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input);
    Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint);
    Mass_Point(float mass_input, sf::Vector2f pos_input)
    
    ~Mass_Point();
    
    void compute_acceleration(sf::Vector2f force);
    void update();
  private:
    
    float mass = m;
    sf::CircleShape point;
    float velocity;
    float acceleration;
    Constraint constraint;
    
};



//ufficiale
Mass_point::Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input, Constraint constraint)   


Mass_Point(float mass_input, sf::Vector2f pos_input, sf::Vector2f velocity_input)
{
  Constraint constraint_input(false, false);
  Mass_point(mass_input, pos_input, velocity_input, constraint_input);
}


Mass_Point(float mass_input, sf::Vector2f pos_input, Constraint constraint)
{
  sf::Vector2f velocity_input(0.0,0.0);
  Mass_point(mass_input, pos_input, velocity_input, constraint);
}

    
Mass_Point(float mass_input, sf::Vector2f pos_input)
{
  Constraint constraint_input(false, false;)
  sf::Vector2f velocity_input(0.0,0.0);
  Mass_point(mass_input, pos_input, velocity_input, constraint_input);
}


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
  for (;;)
  {
    /* code */
  }
  
  return 0;
}


















































