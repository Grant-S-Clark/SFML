#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "SortRect.h"

const int WIDTH = 600;
const int HEIGHT = 400;

inline void swap(int & i, int & j)
{
    int t = i;
    i = j;
    j = t;
}

inline void draw(sf::RenderWindow & window,
          const std::vector< int > & x_pos,
          std::vector< SortRect > & rects)
{
    for (int i = 0, n = rects.size(); i < n; ++i)
    {
        rects.at(i).setPosition(x_pos.at(i));
        window.draw(rects.at(i));
    }

    return;
}

void bubble_sort(sf::RenderWindow &,
                 std::vector< int > &,
                 std::vector< SortRect > &);

int main()
{
    srand((unsigned int)time(NULL));
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sorting");

    window.setFramerateLimit(60);
    
    std::vector< int > x_pos;
    for (int i = 0; i < 50; ++i)
        x_pos.push_back(i * 6 + 5);

    std::vector< SortRect > rects;
    for (int i = 0, n = x_pos.size(); i < n; ++i)
        rects.push_back(SortRect(4, i * 5 + 5));
    SortRect::set_y_pos(WIDTH / 2);

    std::random_shuffle(x_pos.begin(), x_pos.end());

    bool sorted = false;
    
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

        if (!sorted)
        {
            bubble_sort(window, x_pos, rects);
            sorted = true;
        }

        window.clear();
        draw(window, x_pos, rects);
        window.display();
    }
    
    return 0;
}


void bubble_sort(sf::RenderWindow & window,
                 std::vector< int > & x,
                 std::vector< SortRect > & rects)
{
    int n = x.size();
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (x[j] > x[j + 1])
                swap(x[j], x[j + 1]);
            
            rects.at(j + 1).setFillColor(sf::Color::Red);
            rects.at(j).setFillColor(sf::Color::Red);

            window.clear();
            draw(window, x, rects);
            window.display();

            rects.at(j + 1).setFillColor(sf::Color::Green);
            rects.at(j).setFillColor(sf::Color::Green);
        }
    }
    
    return;
}
