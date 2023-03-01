#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
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

//for Linear_Shape
struct Linear_Data;

#define DEFAULT_AMPLITUDE 1.0f
#define DEFAULT_WIDTH 1.0f
#define DEFAULT_REPETITIONS 1.0f
#define DEFAULT_CURRENT_FUNCTION "line"

typedef std::function<void (std::vector<sf::Vector2f>*, float, float, Linear_Data)> linear_method;

// for multiple classes
#define DEFAULT_CONSTRAINT Constraint()
#define DEFAULT_COLOR sf::Color::White


// Classes
// Declaration

class Constraint;
class Mass_Point;
class Gravity;
class Spring;
class Rope;
class Linear_Shape;

// Definition

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
    void set_position(float x, float y) { this->point.setPosition(x,y); };

    void set_constraint(Constraint constraint = DEFAULT_CONSTRAINT) 
    { this->constraint = constraint; };
    void set_constraint(bool x, bool y)
      {
        this->constraint.x = x; 
        this->constraint.y = y;
      };
    Constraint get_constraint() { return constraint; };

    void set_color(sf::Color color = DEFAULT_COLOR) { this->point.setFillColor(color); };
    
    void update(float time_increment);

    sf::CircleShape point;

  private:
    
    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Constraint constraint;

    void print(sf::Time time);

    void builder(float mass, 
                 sf::Vector2f pos, 
                 sf::Vector2f velocity = DEFAULT_VEL, 
                 Constraint constraint = DEFAULT_CONSTRAINT, 
                 sf::Color color = DEFAULT_COLOR);
};


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


class Spring
{
  public:

    Spring(float k, Mass_Point* m1, Mass_Point* m2, int rest_length = DEFAULT_REST_L) 
      { builder(k, m1, m2, rest_length = DEFAULT_REST_L); }

    Spring(float k, Mass_Point* m1, Mass_Point* m2, bool at_rest) 
      { builder(k, m1, m2, get_length(m1, m2)); }


    void set_rest_length(float rest_length) { this->rest_length = rest_length;};
    float get_k() { return k; };
    float get_length() { update_length(); return length;};

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

    float get_length(Mass_Point* m1, Mass_Point* m2) 
    { update_length(m1, m2); return length; };
};


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
  
  Rope(Rope left,
       Rope Right,
       sf::Color color = DEFAULT_COLOR);

  float get_tension() { update_tension(); return tension; }; // return the current medium tension of the rope
  float get_length() { update_length(); return length;};  // return the current length of the rope
  float get_mass() { return mass; }; // return the mass of the rope
  unsigned int size() { return point_number; };

  void set_constraint(std::string point_name, Constraint constraint = DEFAULT_CONSTRAINT); // take a or b (the extremis)
  void set_constraint(std::string point_name, bool x, bool y);                             // and set the constraint
  
  void set_color(sf::Color color); // change the rope color

  void set_shape(Linear_Shape shape, float oscillation_amplitude); // set the position of the rope, oscillational amplitude
                                                                   // are to be passed after normalization (as meters)

  Mass_Point get_mass(unsigned int i); // return a copy of the mass at i
  Spring get_spring(unsigned int i); // return a copy of the spring at i

  sf::Vector2f get_position_at(unsigned int i); // return the position of point i
  sf::Vector2f get_velocity_at(unsigned int i); // return the velocity of point i

  Spring attach_mass(Mass_Point* mass, float k, unsigned int i);

  void set_x_sliding(bool x = true); // set all mass_point.constraint.x except the first and last

  void add_gravity(Gravity* gravity) { gravity->add(&mass_point); }; // add gravity to all mass_point


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


struct Linear_Data
{
  float amplitude;    // from 0 to 1, fraction of requested height for displacement
  float width;        // from 0 to 1, fraction of requested length over wich the function will be applied 
  float repetitions;  // number of repetitions of the function over the width
};

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
  void set_repetitions(float repetitions) {this->data.repetitions = repetitions; };
  void set_width(float width);

  void add_function(std::string name, linear_method foo);

  void change_function(std::string name) { this->current_function = name; };

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


//!________________________________________________________________________________________________________
//! MAIN

int main(int argc, char const *argv[])
{
  //system("rm results.dat");
  sf::Clock global_clock;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "Vibrating string simulation");

  unsigned int mass_point_num = 1000;
  float mass1 = 1.0f;
  float mass2 = 5.0f;
  float time_increment = 0.00001f;
  int ipf = 100;
  float tension = 5.0f;
  float length = 0.625f;
  sf::Vector2f initial_pos1(0.0f,400.0f);
  sf::Vector2f initial_pos2(500.0f,400.0f);
  Constraint constraint_a(true, false);
  Constraint constraint_b(true, true);

  //amplitude, width, repetitions
  Linear_Shape shape("sine", 1.0f, 0.25f, 1.0f);
  Linear_Shape line("line");

  Rope rope1(mass_point_num, mass1, tension, length, initial_pos1, constraint_a, constraint_b);
  Rope rope2(mass_point_num, mass2, tension, length, initial_pos2, constraint_a, constraint_b);

  rope1.set_shape(shape, 0.25f);
  rope2.set_shape(line,0.0f);

  Rope rope(rope1, rope2);

  std::cout << rope1.get_tension() << std::endl;

  //Gravity gravity;

  //rope.add_gravity(&gravity);

  sf::Time global_time;
  sf::Time time;
  sf::Clock clock;
  
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
      rope.apply();
      rope.update(time_increment);
    }

    //gravity.apply();

    window.clear();
    for(unsigned int i=0; i<rope.size(); ++i)
      window.draw(rope.get_mass(i).point);
    window.display();
  }

  //system("code results.dat");
  return 0;
}


//!______________________________________________________________________________________________________________________________________________________________
//! class method

//?______________________________________________________________________________________________________________________________________________________________
//? mass point

Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Vector2f velocity, 
                       Constraint constraint, 
                       sf::Color color)
{  
  builder(mass, position, velocity, constraint, color);
}   


Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       Constraint constraint, 
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}

Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Vector2f velocity,
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}

    
Mass_Point::Mass_Point(float mass, 
                       sf::Vector2f position, 
                       sf::Color color)
{
  builder(mass, position, velocity, constraint, color);
}



void
Mass_Point::builder(float mass, sf::Vector2f position, sf::Vector2f velocity, Constraint constraint, sf::Color color)
{
  point.setRadius(DEFAULT_P_RADIUS);
  point.setPointCount(DEFAULT_C_POINT_COUNT);
  point.setOrigin(DEFAULT_P_RADIUS / 2 , DEFAULT_P_RADIUS / 2);
  point.setPosition(position);
  point.setFillColor(color);

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
Mass_Point::update(float time_increment)
{ 
  if(constraint.x)
    velocity.x = 0.0;
  else
    velocity.x += (acceleration.x * time_increment); 
  
  if(constraint.y)
    velocity.y = 0.0;
  else
    velocity.y += (acceleration.y * time_increment); 
  
   //velocity.x *= 0.9999;
   //velocity.y *= 0.9999;

  sf::Vector2f new_position = point.getPosition();
  
  new_position.x += velocity.x * (time_increment * LENGHT_UNIT);
  new_position.y += velocity.y * (time_increment * LENGHT_UNIT);

  point.setPosition(new_position);
  
  clear_acceleration();
}


//?______________________________________________________________________________________________________________________________________________________________
//? gravity

Gravity::Gravity(std::vector<Mass_Point>* mass_point)
{
  this->g.x = 0.0;
  this->g.y = 9.80665;
  add(mass_point);
}

Gravity::Gravity()
{
  this->g.x = 0.0;
  this->g.y = 9.80665;
}

void
Gravity::add(std::vector<Mass_Point>* mass_point)
{
  for(int i=0; i<mass_point->size(); ++i)
  {
    Mass_Point* temp;
    temp = &mass_point->at(i);
    points.push_back(temp);
    
  }
}

void
Gravity::add(Mass_Point* mass_point)
{
  points.push_back(mass_point);
}

void
Gravity::apply()
{
  for(int i=0; i<points.size(); ++i)
    points.at(i)->update_acceleration(g);
}

//?______________________________________________________________________________________________________________________________________________________________
//? spring

void
Spring::builder(float k, Mass_Point* m1, Mass_Point* m2, float rest_length)
{
  this->k = k;
  this->m1 = m1;
  this->m2 = m2;
  set_rest_length(rest_length);
}

void
Spring::update_vector_length() { update_vector_length(m1, m2); }

void
Spring::update_vector_length(Mass_Point* m1, Mass_Point* m2)
{
  this->vector_length.x = abs(m1->get_position().x * 1000 
                            - m2->get_position().x * 1000) 
                            / (LENGHT_UNIT * 1000);
  this->vector_length.y = abs(m1->get_position().y * 1000 
                            - m2->get_position().y * 1000) 
                            / (LENGHT_UNIT * 1000);
}

void
Spring::update_length() { update_length(m1, m2); }

void
Spring::update_length(Mass_Point* m1, Mass_Point* m2)
{
  this->length = sqrt( pow((m1->get_position().x * 1000 
                     - m2->get_position().x * 1000) / (LENGHT_UNIT * 1000), 2) 
                     + pow((m1->get_position().y * 1000 
                     - m2->get_position().y * 1000) / (LENGHT_UNIT * 1000), 2));
}

void
Spring::apply()
{
  sf::Vector2f force;
  sf::Vector2f a1, a2;
  
  update_length();
  update_vector_length();

  force.x = (length - rest_length) * k * (vector_length.x / length);
  force.y = (length - rest_length) * k * (vector_length.y / length);

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

Rope::Rope(unsigned int point_number, 
           float mass, 
           float tension, 
           float length, 
           sf::Vector2f position , 
           Constraint a, 
           Constraint b, 
           sf::Color color)
{
  builder(point_number, mass, 
          tension, length, position, 
          a, 
          b, 
          color);
}

Rope::Rope(unsigned int point_number, 
           float mass, 
           float tension, 
           float length, 
           sf::Vector2f position, 
           sf::Color color)
{
  builder(point_number, mass, tension, 
          length, position,
           a, 
           b, 
           color);
}

Rope::Rope(Rope left, Rope right, sf::Color color)
{
  this->a = left.get_mass(0).get_constraint();
  this->b = right.get_mass(right.size() -1).get_constraint();

  left.set_constraint("a");
  left.set_constraint("b");
  right.set_constraint("a");
  left.set_constraint("b");

  for(int i = 0; i < left.size(); ++i)
    this->mass_point.push_back(left.get_mass(i));
  
  mass_point.at(left.size()-1).set_constraint();

  for(int i = 0; i < right.size(); ++i)
    this->mass_point.push_back(right.get_mass(i));

  mass_point.at(left.size()).set_constraint();

  for(int i = 0; i < left.size() -1; ++i)
    this->spring.push_back(left.get_spring(i));

  Spring temp(this->spring.at(0).get_k(),
              &mass_point.at(left.size() -1),
              &mass_point.at(left.size()));

  spring.push_back(temp);

  for(int i = 0; i < right.size() -1; ++i)
    this->spring.push_back(right.get_spring(i));

  this->point_number = mass_point.size();
  this->mass = left.get_mass() + right.get_mass();
  update_tension();
  update_length();
  this->position = mass_point.at(0).get_position();
  this->color = color; 
  set_constraint("a");
  set_constraint("b");
}

void 
Rope::builder(unsigned int point_number, float mass, float tension, 
              float length, sf::Vector2f position,  
              Constraint a, Constraint b, sf::Color color)
{
  this->point_number = point_number;
  this->mass = mass;
  this->length = length * LENGHT_UNIT;
  this->tension = tension;
  this->length = length;
  this->position = position;
  this->a = a; 
  this->b = b;
  this->color = color;

  this->mass_point = mass_builder();
  this->spring = spring_builder();
}

std::vector<Mass_Point> 
Rope::mass_builder()
{
  std::vector<Mass_Point> mass_point;

  for(int i = 0; i < point_number; ++i)
  {
    sf::Vector2f temp_pos(position.x + i * length / point_number, position.y);
    
    Mass_Point temp(mass / point_number, temp_pos, color);
 
    mass_point.push_back(temp);
  }

  mass_point.front().set_constraint(a);
  mass_point.back().set_constraint(b);

  return mass_point;
}

std::vector<Spring>
Rope::spring_builder()
{
  std::vector<Spring> spring;
  float k = (tension / length) * point_number;

  for(int i = 0; i < mass_point.size() - 1 ; ++i)
  {
    Spring temp(k, &mass_point.at(i), &mass_point.at(i+1));

    spring.push_back(temp);
  }

  return spring;
}

void
Rope::update_tension()
{
  float force = 0.0f;

  for(int i = 0; i < spring.size(); ++i)
    force = force * (i / i+1) + 
            spring.at(i).get_k() * 
            spring.at(i).get_length() / 
            (i+1);   
  
  this->tension = force;
}

void
Rope::update_length()
{
  this->length = 0;

  for(auto i: spring)
    this->length += i.get_length();
}

void
Rope::set_constraint(std::string point_name, bool x, bool y)
{
  Constraint constraint(x, y);
  set_constraint(point_name, constraint);
}

void
Rope::set_constraint(std::string point_name, Constraint constraint)
{
  const char* name = point_name.c_str();
  if(*name == 'a')
    this->a = constraint;
  else if(*name == 'b')
    this->b = constraint;
}

void
Rope::set_color(sf::Color color)
{
  for(int i = 0; i < mass_point.size(); ++i)
    mass_point.at(i).set_color(color);
}

void
Rope::set_shape(Linear_Shape shape, float oscillation_amplitude)
{
  std::vector<sf::Vector2f> new_position;

  for(int i = 0; i < mass_point.size(); ++i)
    new_position.push_back(mass_point.at(i).get_position());

  shape.apply(&new_position, length,
              oscillation_amplitude);

  for(int i = 0; i < new_position.size(); ++i)
    mass_point.at(i).set_position(new_position.at(i));
}

Mass_Point
Rope::get_mass(unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;  

  return this->mass_point.at(i); 
};


Spring
Rope::get_spring(unsigned int i)
{  
  if(i >= point_number)
    i = point_number -1;

   return this->spring.at(i); 
};


sf::Vector2f
Rope::get_position_at(unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;


  return this->mass_point.at(i).get_position(); 
};

sf::Vector2f
Rope::get_velocity_at(unsigned int i)
{ 
  if(i >= point_number)
    i = point_number -1;

  return this->mass_point.at(i).get_velocity(); 
};


Spring
Rope::attach_mass(Mass_Point* mass, float k, unsigned int i)
{
  if(i >= point_number)
    i = point_number -1;

  Spring new_spring(k, mass, &mass_point.at(i));

  return new_spring;
}

void
Rope::set_x_sliding(bool x)
{
  for(int i = 1; i < mass_point.size() - 1; ++i)
    mass_point.at(i).set_constraint(!x, mass_point.at(i).get_constraint().y);
}

void
Rope::apply()
{
  for(int i = 0; i < spring.size(); ++i)
    spring.at(i).apply();
}

void
Rope::update(float time_increment)
{
  for(int i = 0; i < mass_point.size(); ++i)
    mass_point.at(i).update(time_increment);

  this->position = get_position_at(0);
}

//? ________________________________________________________________________________________________________
//? Linear_Shape

Linear_Shape::Linear_Shape(std::string function_name,
                           float amplitude, 
                           float width, 
                           float repetitions)
{
  builder(function_name, amplitude, width, repetitions);
}

Linear_Shape::Linear_Shape(float amplitude,
             float width,
             float repetitions)
{
  builder(DEFAULT_CURRENT_FUNCTION, amplitude, width, repetitions);
}

void
Linear_Shape::builder(std::string function_name, 
                      float amplitude,
                      float width,
                      float repetitions)
{
  this->current_function = function_name;
  this->data.amplitude = amplitude;
  this->data.width = width;
  this->data.repetitions = repetitions;

  function_loader();  
}



void
Linear_Shape::set_amplitude(float amplitude)
{
  if(amplitude > 1)
    amplitude = 1;
  else if(amplitude < 0)
    amplitude = 0;

  this->data.amplitude = amplitude;
}

void
Linear_Shape::set_width(float width)
{
  if(width > 1)
    width = 1;
  else if(width < 0)
    width = 0;

  this->data.width = width;
}


void
Linear_Shape::add_function(std::string name, linear_method foo)
{
  functions[name] = [this, foo](std::vector<sf::Vector2f>* position, float length, float height)
  { foo(position, length, height, data); };
}

void
Linear_Shape::apply(std::vector<sf::Vector2f>* position, float length, float height)
{
    functions[current_function](position, length, height);
}



void
Linear_Shape::function_loader()
{
  // line distribution
  add_function("line",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(int i = 0; i < position->size(); ++i)
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };
  });

  add_function("sine",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(float i = 1.0f; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
        position->at(i).y = position->at(0).y - std::sin( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) * height * (LENGHT_UNIT * data.amplitude / 2.0f);
    }
  });

  add_function("cosine",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    sf::Vector2f pos0 = position->at(0);

    for(float i = 0; i < position->size(); ++i)
    {
      position->at(i) = { pos0.x + i * length * (LENGHT_UNIT / position->size()), pos0.y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
        position->at(i).y = pos0.y - std::cos( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) * height * (LENGHT_UNIT * data.amplitude / 2.0f);
    }



  });

  add_function("square",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(int i = 1; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {

        if(std::sin(static_cast<float>(i) / position->size() / data.width * data.repetitions * 2.0f * M_PI) < 0)
          position->at(i).y = position->at(0).y - (height * LENGHT_UNIT * data.amplitude / 2.0f);
        else
          position->at(i).y = position->at(0).y + (height * LENGHT_UNIT * data.amplitude / 2.0f);

      }

      position->at(position->size()-1).y = position->at(0).y;
    }
  });

  add_function("triangle",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    for(float i = 1.0f; i < position->size(); ++i)
    {
      position->at(i) = { position->at(0).x + i * length * (LENGHT_UNIT / position->size()), position->at(0).y };

      if(position->at(0).x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {
        float height_increment = height * (LENGHT_UNIT / position->size()) * data.amplitude / data.width * 2.0f * data.repetitions;

        if(std::cos( i / position->size() / data.width * data.repetitions * 2.0f * M_PI) > 0)
          position->at(i).y = position->at(i-1).y + height_increment;
        else  
          position->at(i).y = position->at(i-1).y - height_increment;
      }
    }
  });

  add_function("wave",
  [](std::vector<sf::Vector2f>* position, float length, float height, Linear_Data data)
  {
    sf::Vector2f pos0 = position->at(0);

    for(float i = 0.0f; i < position->size(); ++i)
    {
      position->at(i) = { pos0.x + i * length * (LENGHT_UNIT / position->size()), pos0.y };

      if(pos0.x + i * length * (LENGHT_UNIT / position->size()) < length * data.width * LENGHT_UNIT )
      {
        position->at(i).y = pos0.y - (1 - std::cos( i / position->size() / data.width * data.repetitions * M_PI + M_PI)) * height * (LENGHT_UNIT * data.amplitude / 2);
      }
    }
  });
}