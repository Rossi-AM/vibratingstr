//
// 03/2023
//                  
// Rossi A. M.:  alessandromatteo.rossi@studenti.unimi.it
// Tambini M. :  marco.tambini@studenti.unimi.it
//  
// vstr_advanced.* contain auxiliary classes
// for a vibrating string simulation.
// The elements are Rope and Linear_Shape,
// which includes the struct Linear_Data
// as its own data type.
//

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_basic.h"

// For Linear_Shape
struct Linear_Data;

#define DEFAULT_AMPLITUDE 1.0f
#define DEFAULT_WIDTH 1.0f
#define DEFAULT_REPETITIONS 1.0f
#define DEFAULT_CURRENT_FUNCTION "line"

typedef std::function<void (std::vector<sf::Vector2f>*, float, float, Linear_Data)> linear_method;

//!______________________________________________________________________________________________________________________________________________________________
//! Class declarations

//?______________________________________________________________________________________________________________________________________________________________
//? Linear data: struct responsible for storing linear shape data


struct Linear_Data
{
  float amplitude;    // from 0 to 1, fraction of requested height for displacement
  float width;        // from 0 to 1, fraction of requested length over wich the function will be applied 
  float repetitions;  // number of repetitions of the function over the width
};

//?______________________________________________________________________________________________________________________________________________________________
//? Linear shape: class responsible for distributing a std::vector<sf::Vector2f> in a determined shape along an axis

class Linear_Shape
{ 
  public:
  typedef std::function<void (std::vector<sf::Vector2f>*, float, float)> linear_function; // Declaration of the function type responsible for initializing the positions

  Linear_Shape(std::string function_name = DEFAULT_CURRENT_FUNCTION, 
               float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);

  Linear_Shape(float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);

  void set_amplitude(float amplitude);                                                    // Set the amplitude to the input amplitude (if x<0 set 0, if x>1 set 1)
  void set_repetitions(float repetitions);                                                // Set the repetitions to the input repetitions
  void set_width(float width);                                                            // Set the width to the input width (if x<0 set 0, if x>1 set 1)

  void add_function(std::string name, linear_method foo);                                 // Add a new initialization function passed as argument

  void change_function(std::string name);                                                 // Change the function that will initialize the positions

  void apply(std::vector<sf::Vector2f>* position, float length, float height);            // Initialize the position using the current_function


  private:

  std::string current_function;
  Linear_Data data;

  std::map<std::string, linear_function> functions;                                       // Container of possible functions

  void builder(std::string function_name = DEFAULT_CURRENT_FUNCTION,                      
               float amplitude = DEFAULT_AMPLITUDE,
               float width = DEFAULT_WIDTH,
               float repetitions = DEFAULT_REPETITIONS);                                  // This method is responsible for initializing the class

  void function_loader();                                                                 // Load the initialization function, already included in this library
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
       Rope* Right);

  float get_tension();                                                             // Return the current medium tension of the rope
  float get_length();                                                              // Return the current length of the rope
  float get_mass();                                                                // Return the mass of the rope
  unsigned int size();

  void set_constraint(unsigned int i, Constraint constraint = DEFAULT_CONSTRAINT); // Take a or b (the extremes)
  void set_constraint(unsigned int i, bool x, bool y);                             // and set the constraint
  
  void set_color(sf::Color color = DEFAULT_COLOR);                                 // Change the rope color

  void set_shape(Linear_Shape shape, float oscillation_amplitude);                 // Set the position of the rope, amplitude
                                                                                   // has to be passed after normalization, as meters

  Mass_Point get_mass(unsigned int i);                                             // Return a copy of the mass with i index
  Spring get_spring(unsigned int i);                                               // Return a copy of the spring with i index

  sf::Vector2f get_position_at(unsigned int i);                                    // Return the position of point with i index
  sf::Vector2f get_velocity_at(unsigned int i);                                    // Return the velocity of point with i index

  Spring attach_mass(Mass_Point* mass, float k, unsigned int i);

  void set_x_sliding(bool x = true);                                               // Set all mass_point.constraint.x, except the first and last

  void add_gravity(Gravity* gravity);                                              // Add gravity to all mass_point


  void apply();                                                                    // Apply all springs
  void update(float time_increment);                                               // Update all mass_point positions and velocities


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
               sf::Color color = DEFAULT_COLOR);                                   // This method is responsible for initializing the class

  std::vector<Mass_Point> mass_builder();                                          // This method is responsible for initializing mass_point
  std::vector<Spring> spring_builder();                                            // This method is responsible for initializing spring
  
  void update_tension();                                                           // Compute tension as the average of the forces of all springs
  void update_length();                                                            // Update the lenght as a sum of all lengths
};