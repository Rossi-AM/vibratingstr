#include <iostream>
#include <stdexcept>

#define DEFAULT_VEL 0;
#define DEFAULT_VINCOLI 999;

class CorpoBuilder;
class Corpo {
    private: 
    int massa;
    int x;
    int y;
    int vel;
    int vincoli;
    public:
    Corpo(int massa, int x, int y, int vel, int vincoli);
    void printMe();
    static CorpoBuilder* builder();
};

Corpo::Corpo(int massa, int x, int y, int vel, int vincoli) {
    this->massa = massa;
    this->x = x;
    this->y = y;
    this->vel = vel;
    this->vincoli = vincoli;
}

void Corpo::printMe() {
    std::cout << massa << " " << x << " " << y << " " << vel << " " << vincoli << std::endl;
}

class CorpoBuilder {
    private: 
    int _massa;
    int _x;
    int _y;
    int _vel;
    int _vincoli;
    bool setM;
    bool setP;
    public:
    CorpoBuilder();
    CorpoBuilder* massa(int massa);
    CorpoBuilder* pos(int x, int y);
    CorpoBuilder* vel(int vel);
    CorpoBuilder* vincoli(int vincoli);
    Corpo* build();
};

CorpoBuilder* Corpo::builder() {return new CorpoBuilder();}

CorpoBuilder::CorpoBuilder() {
    _vel = DEFAULT_VEL;
    _vincoli = DEFAULT_VINCOLI;
};

CorpoBuilder* CorpoBuilder::massa(int massa) {
    // Qui puoi anche aggiungere vincoli ai parametri (e.g. massa >= 0)
    _massa = massa; 
    setM = true; return this;
}
CorpoBuilder* CorpoBuilder::pos(int x, int y) {
    _x = x; _y = y; 
    setP = true; return this;
}
CorpoBuilder* CorpoBuilder::vel(int vel) {
    _vel = vel; return this;
}
CorpoBuilder* CorpoBuilder::vincoli(int vincoli) {
    _vincoli = vincoli; return this;
}

Corpo* CorpoBuilder::build() {
    if (!setM || !setP)
        throw std::invalid_argument("Qualcosa non e' stato settato!");
    delete this; // Commit sudoku
    return new Corpo(_massa, _x, _y, _vel, _vincoli);
}

int main(int argc, char const *argv[])
{
    // Da' errore (massa e posizione non settate)
    // Corpo::builder()->build()->printMe();

    // Da' errore (posizione non settata)
    // Corpo::builder()->massa(10)->vel(5)->build()->printMe();

    Corpo* c = Corpo::builder()->massa(10)->pos(1, 1)->build();
    c->printMe();

    Corpo::builder()->massa(15)->pos(10, 0)->vel(5)->build()->printMe();

    // Gli argomenti possono essere dati in qualsiasi ordine...
    Corpo::builder()->massa(100)->vincoli(5)->vel(5)->pos(0, 10)->build()->printMe();

    CorpoBuilder* builder = Corpo::builder()->massa(100)->vincoli(5);
    /* ... O puoi fare altre cose mentre aspetti il resto degli argomenti */
    builder->pos(10, 10)->build()->printMe();

    return 0;
}
