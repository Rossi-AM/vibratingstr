#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

//!____________________________________________________________________________________________________
//class & struct definition

// delimitate "active" position
struct Hotspot
{
    sf::Vector2f top_left, bottom_right;
    sf::Vector2i rect_size;                                 // x: width  y: heigt  
    sf::Vector2f rect_rsize;
    std::string name;

    bool is_hit(sf::Vector2f mouse_rposition);
    void Update_rect_size(sf::Texture* reference);
    void Update_rect_size(sf::RenderWindow* reference);
};

// sprite + hitbox
class Object
{
  public:
  
  Object();
  Object(Hotspot i_hotspot, sf::Texture* texture, sf::RenderWindow* window);
  
  
  Hotspot hitbox;
  std::string name;
  sf::Sprite sprite;

  void Update_position(sf::Vector2i position, sf::RenderWindow* window);

  private:
  Hotspot texture_spot;
  
  void Synchronize_movement(sf::RenderWindow* window);
  void Load_sprite(sf::Texture* texture);
};

//!____________________________________________________________________________________________________
// main function
int main()
{
  sf::Texture texture;
  if(!texture.loadFromFile("virtual_keyboard.png"))
  {
    std::cerr << "texture loading error" << std::endl;
    return 2;
  }
  
  sf::RenderWindow window(sf::VideoMode(1000,600), "click & drag the sprite");

  sf::Sprite back;

  back.setTexture(texture);

  std::vector<Object> object;

  //todo_____________________________________________________________________________________________
  // creating object 
  do
  {
    //todo___________________________________________________________________________________________
    // input borders from file
    std::vector<Hotspot> input_spot;

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
    
      input_spot.push_back(temp_spot);
    }while(!scan.eof());

    scan.close();
    //?______________________________________________________________________________________________

    std::srand(std::time(NULL));

    for(int i=0; i<30; ++i)
    {
      std::cout << "input 'ok' to continue or input a letter (" 
                << i << "/30) for a sprite that will be created with the window: ";
    
      std::string temp_s;
      std::cin >> temp_s;

      if(temp_s == "ok"){break;}

      for(int j=0; j<input_spot.size(); ++j)
      {
        if(input_spot[j].name == temp_s)
        {
          Object temp_o(input_spot[j], &texture, &window);
          object.push_back(temp_o);
        }
      }
    }

    for(int i=0; i<object.size(); ++i)
    {
      sf::Vector2i random_position;
      random_position.x = std::rand()%static_cast<int>(window.getSize().x);
      random_position.y = std::rand()%static_cast<int>(window.getSize().y);
      object[i].Update_position(random_position, &window);
    }
  }while(false);
  //?_______________________________________________________________________________________________

  //todo____________________________________________________________________________________________
  // main cycle
  bool mooving = false;
  int mooving_index = -1;
  sf::Vector2f mouse_click_rcoordinate;

  while(window.isOpen())
  {
    sf::Event event;

    while(window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
      break;
      case sf::Event::MouseButtonPressed:
        if(mooving)
        {
        
          mooving = false;
          mooving_index = -1;
        
        }
        else
        {
        
          mouse_click_rcoordinate.x = static_cast<float>(sf::Mouse::getPosition(window).x) 
                                    / static_cast<float>(window.getSize().x);
          mouse_click_rcoordinate.y = static_cast<float>(sf::Mouse::getPosition(window).y) 
                                    / static_cast<float>(window.getSize().y);

          for(int i=0; i<object.size(); ++i)
          {
            if(object[i].hitbox.is_hit(mouse_click_rcoordinate))
            {
              mooving = true;
              mooving_index = i;
            }
          }          
        
        }
      break;
      case sf::Event::MouseMoved:
        if(mooving)
        object[mooving_index].Update_position(sf::Mouse::getPosition(window), &window);
      break;
      }
    }
    
    window.clear();
    for(int i=0; i<object.size(); ++i)
    {
      window.draw(object[i].sprite);
    }
    window.display();
  }
  //?__________________________________________________________________________________________________
  return 0;
}

//!____________________________________________________________________________________________________
// class & struct  function

//todo_________________________________________________________________________________________________
// Object functions
Object::Object(Hotspot i_hotspot, sf::Texture* texture, sf::RenderWindow* window)
{
    texture_spot = i_hotspot;
    hitbox.name = i_hotspot.name;
    
    texture_spot.Update_rect_size(texture);

    Load_sprite(texture);
    
    hitbox.top_left.x = 0.0;
    hitbox.top_left.y = 0.0;

    hitbox.rect_rsize.x = static_cast<float>(sprite.getTextureRect().width)
                        / static_cast<float>(window->getSize().x);
    hitbox.rect_rsize.y = static_cast<float>(sprite.getTextureRect().height)
                        / static_cast<float>(window->getSize().y);
    
    hitbox.bottom_right.x = hitbox.top_left.x + hitbox.rect_rsize.x;
    hitbox.bottom_right.y = hitbox.top_left.y + hitbox.rect_rsize.y;

    hitbox.Update_rect_size(window);


}

void 
Object::Load_sprite(sf::Texture* texture)
{
  sf::Vector2f rect_vertex;
  rect_vertex.x = texture_spot.top_left.x * texture->getSize().x;
  rect_vertex.y = texture_spot.top_left.y * texture->getSize().y;

  texture_spot.Update_rect_size(texture);

  sprite.setTexture(*texture);
  sprite.setTextureRect(sf::IntRect (static_cast<int>(rect_vertex.x),static_cast<int>(rect_vertex.y),texture_spot.rect_size.x,texture_spot.rect_size.y));
}


void 
Object::Update_position(sf::Vector2i position, sf::RenderWindow* window)
{
  sf::Vector2f center;
  hitbox.top_left.x = static_cast<float>(position.x) 
           / static_cast<float>(window->getSize().x)
           - hitbox.rect_rsize.x / 2;
  hitbox.top_left.y = static_cast<float>(position.y) 
           / static_cast<float>(window->getSize().y)
           - hitbox.rect_rsize.y / 2;

  hitbox.bottom_right.x = hitbox.top_left.x + hitbox.rect_rsize.x;
  hitbox.bottom_right.y = hitbox.top_left.y + hitbox.rect_rsize.y;

  Synchronize_movement(window);
}

void
Object::Synchronize_movement(sf::RenderWindow* window)
{
  sprite.setPosition(hitbox.top_left.x * window->getSize().x,
                     hitbox.top_left.y * window->getSize().y);
}

//todo_________________________________________________________________________________________________
// Hotspot function

bool
Hotspot::is_hit(sf::Vector2f point)
{
  if(top_left.x < point.x && point.x < bottom_right.x &&
     top_left.y < point.y && point.y < bottom_right.y) {return true;}
  return false;
}

void
Hotspot::Update_rect_size(sf::Texture* reference)
{
  rect_size.x = static_cast<int>(bottom_right.x * reference->getSize().x)
              - static_cast<int>(top_left.x * reference->getSize().x);
  rect_size.y = static_cast<int>(bottom_right.y * reference->getSize().y)
              - static_cast<int>(top_left.y * reference->getSize().y);

  rect_rsize.x = static_cast<float>(rect_size.x / reference->getSize().x);
  rect_rsize.y = static_cast<float>(rect_size.y / reference->getSize().y);
}

void
Hotspot::Update_rect_size(sf::RenderWindow* reference)
{
  rect_size.x = static_cast<int>(bottom_right.x * reference->getSize().x)
              - static_cast<int>(top_left.x * reference->getSize().x);
  rect_size.y = static_cast<int>(bottom_right.y * reference->getSize().y)
              - static_cast<int>(top_left.y * reference->getSize().y);

  rect_rsize.x = static_cast<float>(rect_size.x) 
               / static_cast<float>(reference->getSize().x);
  rect_rsize.y = static_cast<float>(rect_size.y) 
               / static_cast<float>(reference->getSize().y);
}