#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define DEFAULT_VEL sf::Vector2f(0.0f, 0.0f)
#define DEFAULT_CONSTRAINT Constraint()
#define NORMAL_POS 800.0f                       // 800 pixels = 1 meter
#define DEFAULT_COLOR sf::Color::White

// Classes

class Constraint
{
  public:
    Constraint() { x = false; y = false; };
    Constraint(bool x, bool y)
      {
        this->x = x;
        this->y = y;
      }

    //~Constraint();

    bool x, y;
};

class Mass_Point
{ 
  public:

    Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint, sf::Color color,);    
    Mass_Point(float mass, sf::Vector2f position, sf::Vector2f velocity, sf::Color color,);
    Mass_Point(float mass, sf::Vector2f position, Constraint constraint, sf::Color color);
    Mass_Point(float mass, sf::Vector2f position, sf::Color color);



    void clear_acceleration();
    void update_acceleration(sf::Vector2f acceleration);

    float get_mass() { return mass; };
    
    sf::Vector2f get_velocity() { return velocity; };
    void set_velocity(sf::Vector2f velocity) { this->velocity = velocity; };
    void set_velocity(float x, float y)
    {
      this->velocity.x = x;
      this->velocity.y = y;
    };
    
    sf::Vector2f get_position() { return point.getPosition(); };
    void set_position(sf::Vector2f position) { this->point.setPosition(position); };
    void setposition(float x, float y) { this->point.setPosition(x,y); };

    void set_constraint(Constraint constraint) { this->constraint = constraint; };
    void set_constraint(bool x, bool y)
      {
        this->constraint.x = x; 
        this->constraint.y = y;
      };
    
    void update(float time_increment, sf::Time global_time);

    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;

    void print(sf::Time time);

    void builder(float mass, sf::Vector2f pos, sf::Color color, sf::Vector2f velocity, Constraint constraint);
};


class Gravity
{
  public:

    Gravity(std::vector<Mass_Point> *mass_point);


    void apply();
  

  private:

    const float g = 9.81;
    std::vector<Mass_Point*> points;

};


class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2) 
      { builder(k, m1, m2, 0); }
    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_lenght) 
      { builder(k, m1, m2, rest_lenght); }
    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest) 
      { builder(k, m1, m2, lenght(m1, m2)); }


    void apply();
    void set_rest_lenght(float rest_lenght) { this->rest_lenght = rest_lenght;};


  private: 
    
    float k;
    float rest_lenght;
    Mass_Point *m1, *m2;

    void builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_lenght);

    float lenght();
    float lenght(Mass_Point* m1, Mass_Point* m2);
    sf::Vector2f vector_lenght();                                                   // output the distance in sf::Vector2f between the extremis of the spring
    sf::Vector2f vector_lenght(Mass_Point* m1, Mass_Point* m2);                     // output the distance in sf::Vector2f between the arguments
};


class Rope ()
{
  public:

  Rope(int point_number, float mass, float initial_tension, 
       float lenght, sf::Vector2f position, 
       Constraint a, Constraint b, sf::Color color);
  
  Rope(int point_number, float mass, float initial_tension, 
       float lenght, sf::Vector2f position);

  Rope(int point_number, float mass, float initial_tension, 
       float lenght, sf::Vector2f position,
       Constraint a, Constraint b,);

  Rope(int point_number, float mass, float initial_tension, 
       float lenght, sf::Vector2f position, sf::Color color);

  Rope(int point_number, float mass, float initial_tension,
       float lenght, sf::Vector2f position);

  void connect_to(Rope *rope); // to connect two ropes

  private:

  void builder(int point_number, float mass, float initial_tension, 
               float lenght, sf::Vector2f position,  
               Constraint a, Constraint b, sf::Color color);

  int point_number;
  float lenght;
  float mass;
  float initial_tension;
  float tension;
  sf::Vector2f position;
  sf::Color color;
  Constraint a, b;
  std::vector<Mass_point> mass_point;
  std::vector<spring> spring;

  //method for initializing positions
}


//!________________________________________________________________________________________________________
//! MAIN

int main(int argc, char const *argv[])
{
  //system("rm results.dat");
  sf::Clock global_clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "when will ale-senpai notice me");

  std::vector<Mass_Point> mass_point;
  int mass_point_num = 1000;
  float mass = 0.0005;
  float k = 5000;
  float time_increment = 0.0001f;
  int ipf = 100;
  int normal_mode = 1;
  
  sf::Time global_time;
  sf::Time time;
  sf::Clock clock;
  
  for(int i = 0; i < mass_point_num; ++i)
  {
    sf::Vector2f position((static_cast<float>(window.getSize().x)/(mass_point_num - 1))*i, static_cast<float>(window.getSize().y)/2);
    Constraint constraint(false, false);

    Mass_Point temp(mass, position, constraint);

    //position.y -= std::sin((position.x / (static_cast<float>(window.getSize().x) / normal_mode) ) * 3.14) * static_cast<float>(window.getSize().y) / 6;

    //temp.set_position(position);

    mass_point.push_back(temp);
  }

  Gravity gravity(&mass_point);
  std::vector<Spring> springs;

  for(int i=0; i<mass_point_num-1; ++i)
  {
    Spring spring(k, &mass_point.at(i), &mass_point.at(i+1));
    springs.push_back(spring);
  }
  
  mass_point.at(0).set_constraint(true, true);
  mass_point.at(mass_point_num-1).set_constraint(true, true);

  clock.restart();
  global_clock.restart();
  //_________________________________________________________________________________________________________
  //main cicle
  while(window.isOpen())
  {
    time = clock.getElapsedTime(); 
    clock.restart();

    global_time = global_clock.getElapsedTime();    
    
    sf::Event event;

    while(window.pollEvent(event))
      if(event.type == sf::Event::Closed) 
        window.close();

    for(int j=0; j<ipf; ++j)
    {
      gravity.apply();

      for(int i=0; i< springs.size(); ++i)
        springs.at(i).apply();
    
      for(int i = 0; i < mass_point.size(); ++i)
        mass_point.at(i).update(time_increment, global_time);
    }

    window.clear();
    for(auto i: mass_point)
      window.draw(i.point);
    window.display();
  }

  //system("code results.dat");
  return 0;
}





//!______________________________________________________________________________________________________________________________________________________________
//! class method

//?______________________________________________________________________________________________________________________________________________________________
//? mass point

Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Color color, sf::Vector2f velocity, Constraint constraint/*, float radius, std::size_t pointCount*/)
{  
  builder(mass, position, velocity, constraint);
}   


Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Color color, sf::Vector2f velocity)
 {
   Constraint constraint(false, false);
   builder(mass, position, velocity, constraint);
 }

Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Color color, Constraint constraint)
 {
   sf::Vector2f velocity(0.0, 0.0);
   builder(mass, position, velocity, constraint);
 }

    
Mass_Point::Mass_Point(float mass, sf::Vector2f position, sf::Color color)
 {
   Constraint constraint(false, false);
   sf::Vector2f velocity(0.0, 0.0);
   builder(mass, position, velocity, constraint);
 }

void
Mass_Point::builder(float mass, sf::Vector2f position, sf::Color color, sf::Vector2f velocity, Constraint constraint)
{
  point.setRadius(3.0/*radius*/);
  point.setPointCount(30/*pointCount*/);
  point.setOrigin(1.5, 1.5);
  point.setPosition(position);

  this->mass = mass;
  this->velocity = velocity;
  clear_acceleration();
  this->constraint = constraint;
}

void
Mass_Point::clear_acceleration()
{
  acceleration.x = 0.0;
  acceleration.y = 0.0;
}

void
Mass_Point::update_acceleration(sf::Vector2f acceleration)
{
  this->acceleration.x += acceleration.x;
  this->acceleration.y += acceleration.y;
}

void 
Mass_Point::print(sf::Time global_time)
{
  std::fstream outfile;
  outfile.open("results.dat", std::ios::app);

  outfile << point.getPosition().x << "\t" << point.getPosition().y 
          << "\t" << velocity.x << "\t" << velocity.y 
          << "\t" << acceleration.x << "\t" << acceleration.y << "\t"
          << global_time.asMicroseconds() << "\n";
}

void
Mass_Point::update(float time_increment, sf::Time global_time)
{ 
  if(constraint.x)
    velocity.x = 0.0;
  else
    velocity.x += (acceleration.x * time_increment); 
  
  if(constraint.y)
    velocity.y = 0.0;
  else
    velocity.y += (acceleration.y * time_increment); 
  
   velocity.x *= 0.9999;
   velocity.y *= 0.9999;

  sf::Vector2f new_position = point.getPosition();
  
  new_position.x += velocity.x * (time_increment * NORMAL_POS);
  new_position.y += velocity.y * (time_increment * NORMAL_POS);

  point.setPosition(new_position);

  //print(global_time);
  
  clear_acceleration();
}




//?______________________________________________________________________________________________________________________________________________________________
//? gravity

Gravity::Gravity(std::vector<Mass_Point> *mass_point)
{
  for(int i=0; i<mass_point->size(); ++i)
  {
    Mass_Point *temp;
    temp = &mass_point->at(i);
    points.push_back(temp);
    
  }
}

void
Gravity::apply()
{
  for(int i=0; i<points.size(); ++i)
  {
    sf::Vector2f acceleration(0.0 , g);
    points.at(i)->update_acceleration(acceleration);
  }
}

//?______________________________________________________________________________________________________________________________________________________________
//? spring

void
Spring::builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_lenght)
{
  this->k = k;
  this->m1 = m1;
  this->m2 = m2;
  set_rest_lenght(rest_lenght);
}

sf::Vector2f
Spring::vector_lenght() { return vector_lenght(m1, m2); }

sf::Vector2f
Spring::vector_lenght(Mass_Point* m1, Mass_Point* m2)
{
  sf::Vector2f lenght;
  lenght.y = abs(m1->get_position().y * 1000 - m2->get_position().y * 1000) / (NORMAL_POS * 1000);
  lenght.x = abs(m1->get_position().x * 1000 - m2->get_position().x * 1000) / (NORMAL_POS * 1000);

  return lenght;
}

float
Spring::lenght() { return lenght(m1, m2); }

float
Spring::lenght(Mass_Point* m1, Mass_Point* m2)
{
  return sqrt( pow((m1->get_position().x * 1000 
                  - m2->get_position().x * 1000) / (NORMAL_POS * 1000), 2) 
             + pow((m1->get_position().y * 1000 
                  - m2->get_position().y * 1000) / (NORMAL_POS * 1000), 2));
}

void
Spring::apply()
{
  sf::Vector2f force;
  sf::Vector2f a1, a2;
  sf::Vector2f vector_lenght = Spring::vector_lenght();
  float lenght = Spring::lenght();

  force.x = (lenght - rest_lenght) * k * (vector_lenght.x / lenght);
  force.y = (lenght - rest_lenght) * k * (vector_lenght.y / lenght);

  a1 = (force / m1->get_mass());
  a2 = ((-force) / m2->get_mass());

  if(m1->get_position().x > m2->get_position().x)
  {
    a1.x *= (-1); 
    a2.x *= (-1); 
  }

  if(m1->get_position().y > m2->get_position().y)
  {
    a1.y *= (-1); 
    a2.y *= (-1); 
  } 

  m1->update_acceleration(a1);
  m2->update_acceleration(a2);
}

//? ________________________________________________________________________________________________________
//? Rope

Rope::Rope(int point_number, float mass, float initial_tension, 
           float lenght, sf::Vector2f position, 
           Constraint a, Constraint b, sf::Color color)
{
  builder(point_number, mass, initial_tension, lenght, 
          position, a, b, Color);
}

Rope::Rope(int point_number, float mass, float initial_tension, 
           float lenght, sf::Vector2f position)
{
  builder(point_number, mass, initial_tension, lenght, 
          position, DEFAULT_CONSTRAINT, DEFAULT_CONSTRAINT, DEFAULT_COLOR)
}

Rope::Rope(int point_number, float mass, float initial_tension, 
           float lenght, sf::Vector2f position,
           Constraint a, Constraint b,)
{
  builder(point_number, mass, initial_tension, lenght, 
          position, a, b, DEFAULT_COLOR);
}

Rope::Rope(int point_number, float mass, float initial_tension, 
           float lenght, sf::Vector2f position, sf::Color color)
{
  builder(point_number, mass, initial_tension, lenght, 
          position, DEFAULT_CONSTRAINT, DEFAULT_CONSTRAINT, color);
}

Rope::Rope(int point_number, float mass, float initial_tension,
           float lenght, sf::Vector2f position)
{
  builder(point_number, mass, initial_tension, lenght, 
          position, DEFAULT_CONSTRAINT, DEFAULT_CONSTRAINT, DEFAULT_COLOR)
}

void 
Rope::builder(int point_number, float mass, float initial_tension, 
              float lenght, sf::Vector2f position,  
              Constraint a, Constraint b, sf::Color color)
{
  
}

void
Rope::connect_to(Rope *rope, )
{
  
}