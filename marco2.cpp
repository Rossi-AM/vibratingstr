#include <iostream>
#include <stdexcept>
#include <vector>

#define DEFAULT_VEL std::vector<float>(0.0f, 0.0f)
#define DEFAULT_CONSTRAINT Constraint()
#define DEFAULT_RADIUS 0.
#define DEFAULT_COUNT 0

class Constraint {};

class Mass_Point {
    private: 
    float mass;
    std::vector<float> pos;
    std::vector<float> velocity;
    Constraint constraint;

    void setup(
        float mass, 
        std::vector<float> pos, 
        std::vector<float> velocity=DEFAULT_VEL,
        Constraint constraint=DEFAULT_CONSTRAINT) 
        {
        this->mass = mass;
        this->pos = pos;
        this->velocity = velocity;
        this->constraint = constraint;
        }

    public:
    Mass_Point(
        float mass, 
        std::vector<float> pos, 
        std::vector<float> velocity=DEFAULT_VEL,
        Constraint constraint=DEFAULT_CONSTRAINT)
        { setup(mass, pos, velocity, constraint); }

    Mass_Point(
        float mass, 
        std::vector<float> pos, 
        Constraint constraint ) 
        { setup(mass, pos, DEFAULT_VEL, constraint); }

};

int main(int argc, char const *argv[])
{
    std::vector<float> v(0.0f,0.0f);

    Mass_Point(0.0f, v);

    Mass_Point(0.0f, v, v); // Velocita'

    Mass_Point(0.0f, v, Constraint()); // Vincoli

    Mass_Point(0.0f, v, v, Constraint()); // Velocita' e Vincoli
    return 0;
}
