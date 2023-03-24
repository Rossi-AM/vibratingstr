
//                                     
// Rossi A. M. - Tambini M.    03/2023 
// 
// In this header the basics of the Vibrating
// string simulator are defined.
// A Mass_Point element is and ideal mass point,
// that is connected by String to another mass
// point.
// Gravity can be applied by using the Gravity
// class. Some point can have a Constraint on
// one, or both, axis (usually the extremes of 
// the Rope, a class that will be defined in
// vstr_advanced.h).
// 

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// For sf::CircleShape
#define DEFAULT_C_POINT_COUNT 30
#define DEFAULT_P_RADIUS 3.0f

// For Mass_Point
#define DEFAULT_VEL sf::Vector2f(0.0f, 0.0f)
#define LENGHT_UNIT 800.0f                       // 800 pixels = 1 meter

// For Spring
#define DEFAULT_REST_L 0.0f

// For multiple classes
#define DEFAULT_CONSTRAINT Constraint()
#define DEFAULT_COLOR sf::Color::White

//!______________________________________________________________________________________________________________________________________________________________
//! class declarations

//?______________________________________________________________________________________________________________________________________________________________
//? Constraint


class Constraint
{
  public:
    Constraint();
    Constraint(bool x, bool y);

    bool x, y;
};

//?______________________________________________________________________________________________________________________________________________________________
//? Mass point: Describe an ideal mass point


class Mass_Point
{ 
  public:

    Mass_Point(float mass, 
               sf::Vector2f position, 
               sf::Vector2f velocity = DEFAULT_VEL, 
               Constraint constraint = DEFAULT_CONSTRAINT, 
               sf::Color color = DEFAULT_COLOR);    

    Mass_Point(float mass, 
               sf::Vector2f position,
               Constraint constraint = DEFAULT_CONSTRAINT, 
               sf::Color color = DEFAULT_COLOR);

    Mass_Point(float mass, 
               sf::Vector2f position, 
               sf::Vector2f velocity = DEFAULT_VEL,
               sf::Color color = DEFAULT_COLOR);

    Mass_Point(float mass,
               sf::Vector2f position,
               sf::Color color = DEFAULT_COLOR);




    void clear_acceleration();                              // Set acceleration value at 0 (it'll be used whenever there's no jerk, so, everytime)
    void update_acceleration(sf::Vector2f acceleration);    // Add the input acceleration to the existing acceleration

    float get_mass();                                       // Return mass value
    
    sf::Vector2f get_velocity();                            // Return (vx, vy) as a sf::Vector2f
    void set_velocity(sf::Vector2f velocity);               // Change the velocity to the input velocity
    void set_velocity(float x, float y);                    

    
    sf::Vector2f get_position();                            // Return (x, y) as a sf::Vector2f
    void set_position(sf::Vector2f position);               // Change the position to the input position
    void set_position(float x, float y);

    void set_constraint(Constraint constraint = DEFAULT_CONSTRAINT); // Change the constraint to the input constraint
    void set_constraint(bool x, bool y);
    Constraint get_constraint();                            // Return the constraint as a Constraint element

    void set_color(sf::Color color = DEFAULT_COLOR);        // Change the color to the input color
    
    void update(float time_increment);                      // Update velocity and position by the time increment using 
                                                            // Euler's method approximated at first order

    sf::CircleShape point;                                  // SFML object containing position and drawing elements

  private:
    
    float mass;                                             
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;

    void builder(float mass,
                 sf::Vector2f pos, 
                 sf::Vector2f velocity = DEFAULT_VEL, 
                 Constraint constraint = DEFAULT_CONSTRAINT, 
                 sf::Color color = DEFAULT_COLOR);           // Method responsible for initializing the class    
};

//?______________________________________________________________________________________________________________________________________________________________
//? Gravity: classical gravity towards the ground, with acceleration g = 9.80665  

class Gravity
{
  public:

  Gravity(std::vector<Mass_Point>* mass_point);
  Gravity();

  void add(std::vector<Mass_Point>* mass_point);             // Add a/some mass/masses to which apply gravity
  void add(Mass_Point* mass_point);

  void apply();                                              // Update the accelerations of the masses, adding gravity
  

  private:

  sf::Vector2f g;
  std::vector<Mass_Point*> points;

};

//?______________________________________________________________________________________________________________________________________________________________
//? Spring: ideal massless spring

class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_length = DEFAULT_REST_L);
    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest);

    void set_rest_length(float rest_length);                 // Set the rest length, which corresponds to a null force
    void set_k(float k);                                     // Set the spring elastic constant
    float get_k();                                           // Return the spring elastic constant
    float get_length();                                      // Return the current length of the spring

    void apply();                                            // Rpdate the acceleration of the two masses to which the spring is linked

  private: 
    
    float k;                                               
    float rest_length;
    float length;
    sf::Vector2f vector_length;
    Mass_Point* m1;
    Mass_Point* m2;

    void builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_length);      // Method responsible for initializing the class

    void update_vector_length();                                                   // Update vector_length as the distance between the two masses
    void update_vector_length(Mass_Point* m1, Mass_Point* m2);                     // Update vector_length as the distance between two input masses

    void update_length();                                                          // Update length as the distance between the two masses
    void update_length(Mass_Point* m1, Mass_Point* m2);                            // Update length as the distance between two input masses

    float get_length(Mass_Point* m1, Mass_Point* m2);                              // Return the length between two input masses (used for initialization)
};