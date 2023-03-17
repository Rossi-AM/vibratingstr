#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

#define DEFAULT_NORMALIZATION false
#define DEFAULT_NORMAL_HEIGHT 400.0f

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
    
    std::vector<sf::Vector2f> evaluate_at_time(float time, 
                                               float y0 = DEFAULT_NORMAL_HEIGHT,
                                               bool to_normalize = DEFAULT_NORMALIZATION, 
                                               float normal_height = DEFAULT_NORMAL_HEIGHT);
    std::vector<FT_DATA> evaluate_all_time(float y0 = DEFAULT_NORMAL_HEIGHT,
                                           bool to_normalize = DEFAULT_NORMALIZATION, 
                                           float normal_height = DEFAULT_NORMAL_HEIGHT);
  private:

    std::vector<FT_DATA> time_data;

    std::vector<sf::Vector2f> evaluate(std::vector<sf::Vector2f> point, float y0);
    std::vector<std::complex<double>> transform(std::vector<std::complex<double>> point);
    std::vector<sf::Vector2f> normalize(std::vector<sf::Vector2f> point, float y0 = DEFAULT_NORMAL_HEIGHT, float normal_height = DEFAULT_NORMAL_HEIGHT);
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