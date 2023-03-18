#include <complex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "vstr_advanced.h"

#define DEFAULT_NORMALIZATION false
#define DEFAULT_NORMAL_HEIGHT 400.0f

//?______________________________________________________________________________________________________________________________________________________________
//? FFT data time: store the shape of the rope at a give time

struct FT_DATA
{
  unsigned int index;
  float time;
  std::vector<sf::Vector2f> position;
};

//?______________________________________________________________________________________________________________________________________________________________
//? DFT: class for calculating the discrete fourier transform of the rope (every point represent a normal mode of the rope)

class DFT
{
  public:

    void input(sf::Vector2f position, float time);                                              // input position for a given time, if time is not foud create a new FT_DATA for that time
    void input(std::vector<sf::Vector2f> position, float time);
    
    std::vector<sf::Vector2f> evaluate_at_time(float time, 
                                               float y0 = DEFAULT_NORMAL_HEIGHT,
                                               bool to_normalize = DEFAULT_NORMALIZATION, 
                                               float normal_height = DEFAULT_NORMAL_HEIGHT);    // return the fourier transform at the fiven time
    std::vector<FT_DATA> evaluate_all_time(float y0 = DEFAULT_NORMAL_HEIGHT,
                                           bool to_normalize = DEFAULT_NORMALIZATION, 
                                           float normal_height = DEFAULT_NORMAL_HEIGHT);        // return the fourier transform for all saved times

    void normalize(std::vector<sf::Vector2f> *point, float y0 = DEFAULT_NORMAL_HEIGHT, float normal_height = DEFAULT_NORMAL_HEIGHT);  // make the output better visualizzable on a sfml window
  private:

    std::vector<FT_DATA> time_data;

    void evaluate(std::vector<sf::Vector2f> *point, float y0 = DEFAULT_NORMAL_HEIGHT);          // prepare the data for the DFT
    std::vector<std::complex<double>> transform(std::vector<std::complex<double>> point);       // the DFT algorithm
};

//?______________________________________________________________________________________________________________________________________________________________
//? FFT: class for computing the fast fourier transform of the rope (every point represent a even normal mode of the rope)

class FFT : public DFT
{  
  private:

    std::vector<FT_DATA> time_data;

    void evaluate(std::vector<sf::Vector2f> *point, float y0 = DEFAULT_NORMAL_HEIGHT);          // prepare the data for the FFT
    std::vector<std::complex<double>>  transform(std::vector<std::complex<double>> point);      // the FFT algorithm by Cooly & Tukey
};