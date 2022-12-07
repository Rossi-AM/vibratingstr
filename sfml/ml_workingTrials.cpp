#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1080, 2060), "SFML works!");
    sf::RenderWindow window_1(sf::VideoMode(1080, 2060), "SFML works 2!");
    window_1.close();
    sf::CircleShape shape(500.f);
    sf::CircleShape shape_1(500.f);
    shape.setFillColor(sf::Color::Green);
    shape_1.setFillColor(sf::Color::Red);
    int control = 0;    

    while (window.isOpen())
    {
        sf::Event event;
        sf::Event event_1;
        while (window.pollEvent(event) || window_1.pollEvent(event_1))
        {
            if (event.type == sf::Event::Closed && control == 0)
            {
                ++control; 
                window_1.create(sf::VideoMode(1080, 2060), "SFML works 2!");
            }
            if(event_1.type == sf::Event::MouseButtonPressed && control == 1)
                ++control;
            if( control == 2)
            {   
                window.close();
            }
        }

        if (control == 1)
        {
            window_1.clear();
            window_1.draw(shape_1);
            window_1.display();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}