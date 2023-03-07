#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_basic.h"

//for Linear_Shape
struct Linear_Data;

#define DEFAULT_AMPLITUDE 1.0f
#define DEFAULT_WIDTH 1.0f
#define DEFAULT_REPETITIONS 1.0f
#define DEFAULT_CURRENT_FUNCTION "line"

typedef std::function<void (std::vector<sf::Vector2f>*, float, float, Linear_Data)> linear_method;

//!______________________________________________________________________________________________________________________________________________________________
//! class declarations

//?______________________________________________________________________________________________________________________________________________________________
//? linear data


struct Linear_Data
{
  float amplitude;    // from 0 to 1, fraction of requested height for displacement
  float width;        // from 0 to 1, fraction of requested length over wich the function will be applied 
  float repetitions;  // number of repetitions of the function over the width
};

//?______________________________________________________________________________________________________________________________________________________________
//? linear shape

class Linear_Shape
{ 
  public:
  typedef std::function<void (std::vector<sf::Vector2f>*, float, float)> linear_function;

  Linear_Shape(std::string function_name = DEFAULT_CURRENT_FUNCTION, 
               float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);

  Linear_Shape(float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);

  void set_amplitude(float amplitude);
  void set_repetitions(float repetitions);
  void set_width(float width);

  void add_function(std::string name, linear_method foo);

  void change_function(std::string name);

  void apply(std::vector<sf::Vector2f>* position, float length, float height);


  private:

  std::string current_function;
  Linear_Data data;

  std::map<std::string, linear_function> functions;

  void builder(std::string function_name = DEFAULT_CURRENT_FUNCTION, 
               float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);

  void function_loader();
};

//?______________________________________________________________________________________________________________________________________________________________
//? rope

class Rope
{
  public:

  Rope(unsigned int point_number, 
       float mass, 
       float tension, 
       float length, 
       sf::Vector2f position, 
       Constraint a = DEFAULT_CONSTRAINT, 
       Constraint b = DEFAULT_CONSTRAINT, 
       sf::Color color = DEFAULT_COLOR);
  
  Rope(unsigned int point_number, 
       float mass, 
       float tension, 
       float length, 
       sf::Vector2f position, 
       sf::Color color = DEFAULT_COLOR);
  
  Rope(Rope* left,
       Rope* Right,
       sf::Color color = DEFAULT_COLOR);

  float get_tension(); // return the current medium tension of the rope
  float get_length();  // return the current length of the rope
  float get_mass(); // return the mass of the rope
  unsigned int size();

  void set_constraint(unsigned int i, Constraint constraint = DEFAULT_CONSTRAINT); // take a or b (the extremis)
  void set_constraint(unsigned int i, bool x, bool y);                             // and set the constraint
  
  void set_color(sf::Color color = DEFAULT_COLOR); // change the rope color

  void set_shape(Linear_Shape shape, float oscillation_amplitude); // set the position of the rope, oscillational amplitude
                                                                   // are to be passed after normalization (as meters)

  Mass_Point get_mass(unsigned int i); // return a copy of the mass at i
  Spring get_spring(unsigned int i); // return a copy of the spring at i

  sf::Vector2f get_position_at(unsigned int i); // return the position of point i
  sf::Vector2f get_velocity_at(unsigned int i); // return the velocity of point i

  Spring attach_mass(Mass_Point* mass, float k, unsigned int i);

  void set_x_sliding(bool x = true); // set all mass_point.constraint.x except the first and last

  void add_gravity(Gravity* gravity); // add gravity to all mass_point


  void apply(); // apply all springs
  void update(float time_increment); // update all mass_point positions and velocity


  private:

  unsigned int point_number;
  float length;
  float mass;
  float tension;
  sf::Vector2f position;
  sf::Color color;
  Constraint a, b;
  std::vector<Mass_Point> mass_point;
  std::vector<Spring> spring;

  void builder(unsigned int point_number, 
               float mass, 
               float tension, 
               float length, 
               sf::Vector2f position,  
               Constraint a = DEFAULT_CONSTRAINT, 
               Constraint b = DEFAULT_CONSTRAINT, 
               sf::Color color = DEFAULT_COLOR);

  std::vector<Mass_Point> mass_builder();
  std::vector<Spring> spring_builder();
  
  void update_tension();
  void update_length();
};