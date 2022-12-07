#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>

// Classes

class Mass_Point
{ 
  public:

    Mass_Point(float mass_input, bool isBound_input, sfml::Vector2F pos_input, sfml::Vector2F velocity_input);    
    Mass_Point(float mass_input, sfml::Vector2F pos_input, sfml::Vector2F velocity_input);
    Mass_Point(float mass_input, sfml::Vector2F pos_input, bool isBound_input);
    Mass_Point(float mass_input, sfml::Vector2F pos_input)
    void update();
    
    
  private:
    
    float mass = m;
    sfml::CircleShape point;
    float velocity;
    float acceleration;
    bool isBound;
    
    
};



//ufficiale
Mass_point::Mass_Point(float mass_input, bool isBound_input, sfml::Vector2F pos_input, sfml::Vector2F velocity_input);    

Mass_Point::Mass_Point(float mass_input, sfml::Vector2F pos_input, sfml::Vector2F velocity_input)
{
  Mass_point(mass_input, false, pos_input, velocity_input);
}

Mass_Point::Mass_Point(float mass_input, sfml::Vector2F pos_input, bool isBound_input)
{
  sfml::Vector2F velocity_input(0.0,0.0);
  Mass_point(mass_input, isBound_input, pos_input, velocity_input);
}

Mass_Point::Mass_Point(float mass_input, sfml::Vector2F pos_input)
{
  sfml::Vector2F velocity_input(0.0,0.0);
  Mass_point(mass_input, false, pos_input, 0); //correggere questo 0
}



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

class rope ()
{
  

  private:
    float head_pos;
    float tail_pos;
    float lenght = std::abs(head_pos - tail_pos);
    int point_number;
    float total_mass;

}




















































