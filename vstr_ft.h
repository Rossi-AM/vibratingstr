#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

//?______________________________________________________________________________________________________________________________________________________________
//? FFT data time

struct FT_DATA
{
  unsigned int index;
  float time;
  std::vector<sf::Vector2f> position;
};

//?______________________________________________________________________________________________________________________________________________________________
//? DFT 

class DFT
{
  public:

    void input(sf::Vector2f position, float time);
    void input(std::vector<sf::Vector2f> position, float time);
    
    std::vector<sf::Vector2f> evaluate_at_time(float time);
    std::vector<FT_DATA> evaluate_all_time();
  
  private:

    std::vector<FT_DATA> time_data;

    std::vector<sf::Vector2f> evaluate(std::vector<sf::Vector2f> point);
    std::vector<std::complex<double>> transform(std::vector<std::complex<double>> point);
};

//?______________________________________________________________________________________________________________________________________________________________
//? FFT 

class FFT : public DFT
{  
  private:

    std::vector<FT_DATA> time_data;

    std::vector<sf::Vector2f> evaluate(std::vector<sf::Vector2f> point);
    std::vector<std::complex<double>> transform(std::vector<std::complex<double>> point);
};