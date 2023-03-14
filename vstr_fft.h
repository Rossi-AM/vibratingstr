#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

//?______________________________________________________________________________________________________________________________________________________________
//? FFT data time

struct FFT_Data
{
  unsigned int index;
  float time;
  std::vector<sf::Vector2f> position;
};

//?______________________________________________________________________________________________________________________________________________________________
//? FFT 

class FFT
{
  public:

    void input(sf::Vector2f position, float time, unsigned int index);

    void input_by_time(sf::Vector2f position, float time);
    void input_by_time(std::vector<sf::Vector2f> position, float time);
    
    std::vector<sf::Vector2f> evaluate_at_time(float time);
    std::vector<sf::Vector2f> evaluate_for_index(unsigned int index);
    std::vector<FFT_Data> evaluate_all_time();
  
  private:

    std::vector<FFT_Data> time_data;
    std::vector<FFT_Data> index_data;

    std::vector<sf::Vector2f> evaluate(std::vector<sf::Vector2f> point);
    std::vector<std::complex<double>> transform(std::vector<std::complex<double>> point);
};

