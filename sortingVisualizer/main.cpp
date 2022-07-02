#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "SortRect.h"

const int WIDTH = 600;
const int HEIGHT = 400;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sorting");
    
    std::vector< int > x_pos;
    for (int i = 0; i < 50; ++i)
        x_pos.push_back(i * 6 + 5);

    std::vector< SortRect > rects;
    for (int i = 0, n = x_pos.size(); i < n; ++i)
        rects.push_back(SortRect(4, i * 5 + 5));
    SortRect::set_y_pos(WIDTH / 2);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.clear();
        
        for (int i = 0, n = rects.size(); i < n; ++i)
        {
            rects.at(i).setPosition(x_pos.at(i));
            window.draw(rects.at(i));
        }
        
        window.display();
    }
    
    return 0;
}
