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
//? mass point


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




    void clear_acceleration();
    void update_acceleration(sf::Vector2f acceleration);

    float get_mass();
    
    sf::Vector2f get_velocity();
    void set_velocity(sf::Vector2f velocity);
    void set_velocity(float x, float y);

    
    sf::Vector2f get_position();
    void set_position(sf::Vector2f position);
    void set_position(float x, float y);

    void set_constraint(Constraint constraint = DEFAULT_CONSTRAINT);
    void set_constraint(bool x, bool y);
    Constraint get_constraint();

    void set_color(sf::Color color = DEFAULT_COLOR);
    
    void update(float time_increment);

    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;

    void builder(float mass, 
                 sf::Vector2f pos, 
                 sf::Vector2f velocity = DEFAULT_VEL, 
                 Constraint constraint = DEFAULT_CONSTRAINT, 
                 sf::Color color = DEFAULT_COLOR);
};

//?______________________________________________________________________________________________________________________________________________________________
//? gravity

class Gravity
{
  public:

  Gravity(std::vector<Mass_Point>* mass_point);
  Gravity();

  void add(std::vector<Mass_Point>* mass_point);
  void add(Mass_Point* mass_point);

  void apply();
  

  private:

  sf::Vector2f g;
  std::vector<Mass_Point*> points;

};

//?______________________________________________________________________________________________________________________________________________________________
//? spring

class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_length = DEFAULT_REST_L);
    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest);

    void set_rest_length(float rest_length);
    void set_k(float k);
    float get_k();
    float get_length();

    void apply();

  private: 
    
    float k;
    float rest_length;
    float length;
    sf::Vector2f vector_length;
    Mass_Point* m1;
    Mass_Point* m2;

    void builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_length);

    void update_vector_length();                                                   // output the distance in sf::Vector2f between the extremis of the spring
    void update_vector_length(Mass_Point* m1, Mass_Point* m2);                     // output the distance in sf::Vector2f between the arguments

    void update_length();
    void update_length(Mass_Point* m1, Mass_Point* m2);

    float get_length(Mass_Point* m1, Mass_Point* m2);
};