#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

//todo MESSAGE HANDLER (here's handler-1, do you hear me spearhead squadron?)
class handler
{
  public:
    std::string get_message(){return current_message;};
    void set_message()
    {
      std::string temp;
      std::getline(std::cin,temp);
      current_message = temp;
    };
    void set_message(std::string message){current_message = message;};

    void set_window(sf::RenderWindow *window_pointer){window = window_pointer;};
    
    sf::RenderWindow* window;
  private:
    std::string current_message = " ";
};

//todo RENDER FUNCTION
void render_function(handler *handler1)
{
  handler1->window->create(sf::VideoMode(1080, 2060), "Text & thread drawnings trials");
  sf::Font arial;
  sf::Text text;
  
  arial.loadFromFile("arial.ttf");
  text.setFont(arial);
  text.setString("Hello World");

  while(handler1->window->isOpen())
  {
    sf::Event event;
    
    while(handler1->window->pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
        handler1->window->close();
    }

    if(handler1->get_message() != "exit" && handler1->get_message() != "Exit")
    {
      text.setString(std::string(handler1->get_message()));
      text.setCharacterSize(64);
      text.setFillColor(sf::Color::Red);
      handler1->window->clear();
      handler1->window->draw(text);
      handler1->window->display();
    }else{
      handler1->window->close();
    }
  }
}

//todo MAIN
int main()
{
  sf::Clock clock;
  handler handler1;
  sf::RenderWindow window;

  handler1.set_message("hola~~");
  handler1.set_window(&window);


  sf::Thread rendering_thread(&render_function, &handler1);
  rendering_thread.launch();

  while(clock.getElapsedTime() < sf::seconds(2)){}

  while(window.isOpen())
    handler1.set_message();

  return 0;
}