#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Hotspot{

  Hotspot(){};
  Hotspot(sf::Vector2f top_left_i, sf::Vector2f bottom_right_i, 
          std::string name_i)
         {
           top_left = top_left_i; 
           bottom_right = bottom_right_i;
           name = name_i;
         }

  sf::Vector2f top_left, bottom_right;
  std::string name;
  
  void check_boundaries(sf::Vector2f point);
};

int main()
{
  //global variable
  sf::Texture texture;
  if(!texture.loadFromFile("virtual_keyboard.png"))
  {
    std::cerr << "texture loading error" << std::endl;
    return 2;
  }
  sf::RenderWindow window(sf::VideoMode(texture.getSize().x,texture.getSize().y), "input window");
  
  sf::Sprite sprite;
  sprite.setTexture(texture,true);

  sf::Vector2f mouse_click_rcoordinate;

  std::vector<Hotspot> hotspot;

  std::fstream scan;
  scan.open("texture_boundaries.dat",std::ios::in);
  if(scan.good())
  {
    do
    {
      Hotspot temp_spot;
      scan >> temp_spot.name 
           >> temp_spot.top_left.x >> temp_spot.top_left.y 
           >> temp_spot.bottom_right.x >> temp_spot.bottom_right.y;

      hotspot.push_back(temp_spot);
    }while(!scan.eof());
  }

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

        std::string temp_s;
        bool new_spot=true;

        std::cout << "what is this hotspot name: ";
        std::cin >> temp_s;

        for(int i=0; i<hotspot.size(); ++i)
        {
          if(temp_s == hotspot[i].name)
          {
            hotspot[i].check_boundaries(mouse_click_rcoordinate);
            new_spot = false;
          }
        }

        if(new_spot)
        {
          Hotspot temp_spot(mouse_click_rcoordinate, mouse_click_rcoordinate, temp_s);
          hotspot.push_back(temp_spot);
        }
      }
    }

    window.clear();
    window.draw(sprite);
    window.display();
  }

  system("rm texture_boundaries.dat");

  std::fstream print;
  print.open("texture_boundaries.dat",std::ios::out);
  if(!print.good())
  {
    std::cerr << "file creation/opening error" << std::endl;
    return 1;
  }

  for(int i=1; i<hotspot.size(); ++i)
  {
    print << hotspot[i].name << ' '
          << hotspot[i].top_left.x << ' ' 
          << hotspot[i].top_left.y << ' '
          << hotspot[i].bottom_right.x << ' '
          << hotspot[i].bottom_right.y << ' '
          << std::endl;
  }

  print.close();

  return 0;
}

void 
Hotspot::check_boundaries(sf::Vector2f point)
{
    if(top_left.x > point.x) top_left.x = point.x;
    if(top_left.y > point.y) top_left.y = point.y;
    if(bottom_right.x < point.x) bottom_right.x = point.x;
    if(bottom_right.y < point.y) bottom_right.y = point.y;
}