#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Hotspot{
  sf::Vector2f top_left, bottom_right;
  std::string name;
  
  bool check_boundaries(sf::Vector2f point);
};


int main()
{
    system("clear");

  //global variable
  sf::RenderWindow window(sf::VideoMode(440,210), "input window");
  sf::Texture texture;
  if(!texture.loadFromFile("virtual_keyboard.png"))
  {
    std::cerr << "texture loading error" << std::endl;
    return 2;
  }

  sf::Sprite sprite;
  sprite.setTexture(texture,true);

  sf::Vector2f mouse_click_rcoordinate;

  std::vector<Hotspot> hotspot;

  std::fstream scan;
  scan.open("texture_boundaries.dat",std::ios::in);
  if(!scan.good())
  {
    std::cerr << "file creation/opening error" << std::endl;
    return 1;
  }

  do
  {
    Hotspot temp_spot;
    scan >> temp_spot.name 
         >> temp_spot.top_left.x >> temp_spot.top_left.y 
         >> temp_spot.bottom_right.x >> temp_spot.bottom_right.y;
    
    hotspot.push_back(temp_spot);
  }while(!scan.eof());

  scan.close();

  //?____________________________________________________________________________________________________________________
  //main cicle
  while(window.isOpen())
  {
    sf::Event event;

    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
      {
          window.close();
      }
      
      if(event.type == sf::Event::MouseButtonPressed)
      {
        mouse_click_rcoordinate.x = static_cast<float>(sf::Mouse::getPosition(window).x) 
                                   / static_cast<float>(window.getSize().x);
        mouse_click_rcoordinate.y = static_cast<float>(sf::Mouse::getPosition(window).y) 
                                   / static_cast<float>(window.getSize().y);

        for(int i=0; i<hotspot.size(); ++i)
        {
          if(hotspot[i].check_boundaries(mouse_click_rcoordinate))
          {
            if(hotspot[i].name == "space")
            {
              std::cout << ' ';
            } else if(hotspot[i].name == "enter") {
              std::cout << std::endl;
            } else if(hotspot[i].name == "tab") {
              std::cout << "    ";
            } else if(hotspot[i].name == "maiusc" || hotspot[i].name == "caps") {
            } else {
              std::cout << hotspot[i].name;
            }
            
            /*
            if(hotspot[i].name == "space")
            {
              std::printf(" ");
            } else if(hotspot[i].name == "enter") {
              std::printf("\n");
            } else if(hotspot[i].name == "tab") {
              std::printf("    ");
            } else if(hotspot[i].name == "maiusc" || hotspot[i].name == "caps") {
            } else {
              const char* temp_c = hotspot[i].name.c_str();
              std::printf("%s",temp_c);
            }
            */
          }
        }
      }
    }

    window.clear();
    window.draw(sprite);
    window.display();
  }

  std::cout << std::endl;
  return 0;
}

bool
Hotspot::check_boundaries(sf::Vector2f point)
{
  if( point.x < bottom_right.x && point.x > top_left.x &&
      point.y > bottom_right.y && point.y < top_left.y) 
      return true;
  return false;
}