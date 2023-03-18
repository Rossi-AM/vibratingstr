#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// for sf::CircleShape
#define DEFAULT_C_POINT_COUNT 30
#define DEFAULT_P_RADIUS 3.0f

// for Mass_Point
#define DEFAULT_VEL sf::Vector2f(0.0f, 0.0f)
#define LENGHT_UNIT 800.0f                       // 800 pixels = 1 meter

// for Spring
#define DEFAULT_REST_L 0.0f

// for multiple classes
#define DEFAULT_CONSTRAINT Constraint()
#define DEFAULT_COLOR sf::Color::White

//!______________________________________________________________________________________________________________________________________________________________
//! class declarations

//?______________________________________________________________________________________________________________________________________________________________
//? constraint


class Constraint
{
  public:
    Constraint();
    Constraint(bool x, bool y);

    bool x, y;
};

//?______________________________________________________________________________________________________________________________________________________________
//? mass point: secribe an ideal point mass


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




    void clear_acceleration();                              // set acceleration value at 0 (to use at every time step without jerk)
    void update_acceleration(sf::Vector2f acceleration);    // add the input acceleration to the existing acceleration

    float get_mass();                                       // return mass value
    
    sf::Vector2f get_velocity();                            // return (vx, vy) as a sf::Vector2f
    void set_velocity(sf::Vector2f velocity);               // change the velocity to the input velocity
    void set_velocity(float x, float y);                    

    
    sf::Vector2f get_position();                            // return (x, y) as a sf::Vector2f
    void set_position(sf::Vector2f position);               // change the position to the input position
    void set_position(float x, float y);

    void set_constraint(Constraint constraint = DEFAULT_CONSTRAINT); // change the constraint to the input constraint
    void set_constraint(bool x, bool y);
    Constraint get_constraint();                            // return the constraint as a Constraint

    void set_color(sf::Color color = DEFAULT_COLOR);        // change the color to the input color
    
    void update(float time_increment);                      // update velocity and position by the time increment using 
                                                            // Euler's method aproximated at first order

    sf::CircleShape point;                                  // sfml object containing position and drawings elements

  private:
    
    float mass;                                             
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;

    void builder(float mass,
                 sf::Vector2f pos, 
                 sf::Vector2f velocity = DEFAULT_VEL, 
                 Constraint constraint = DEFAULT_CONSTRAINT, 
                 sf::Color color = DEFAULT_COLOR);           // method responsible of initializing the class    
};

//?______________________________________________________________________________________________________________________________________________________________
//? gravity: classical gravity toward down with acceleration g = 9.80665  

class Gravity
{
  public:

  Gravity(std::vector<Mass_Point>* mass_point);
  Gravity();

  void add(std::vector<Mass_Point>* mass_point);             // add a/some mass/masses to wich apply gravity
  void add(Mass_Point* mass_point);

  void apply();                                              // update the accelerations of the masses adding gravity
  

  private:

  sf::Vector2f g;
  std::vector<Mass_Point*> points;

};

//?______________________________________________________________________________________________________________________________________________________________
//? spring: ideal massless spring

class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_length = DEFAULT_REST_L);
    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest);

    void set_rest_length(float rest_length);                 // set the rest length at wich the force is 0
    void set_k(float k);                                     // set the spring constant
    float get_k();                                           // return the spring constrant
    float get_length();                                      // return the current length of the spring

    void apply();                                            // update the acceleration of the two masses to wich the spring is attached

  private: 
    
    float k;                                               
    float rest_length;
    float length;
    sf::Vector2f vector_length;
    Mass_Point* m1;
    Mass_Point* m2;

    void builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_length);      // method responsible of initializing the class

    void update_vector_length();                                                   // update vector_length as the distance between the two masses
    void update_vector_length(Mass_Point* m1, Mass_Point* m2);                     // update vector_length as the distance between two input masses

    void update_length();                                                          // update length as the distance between the two masses
    void update_length(Mass_Point* m1, Mass_Point* m2);                            // update length as the distance between two input masses

    float get_length(Mass_Point* m1, Mass_Point* m2);                              // return the length between two input masses (used for initialization)
};