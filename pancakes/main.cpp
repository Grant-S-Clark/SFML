#include <iostream>
#include <cstdlib>           //rand
#include <vector>            //std::vector
#include <ctime>             //time(NULL)
#include <algorithm>         //std::random_shuffle
#include <SFML/Graphics.hpp> //Graphics

const int MAX_PANCAKES       = 10;
const int MIN_PANCAKES       = 2;
const int PANCAKE_HEIGHT     = 15;
const int PANCAKE_WIDTH_MULT = 40;
const int WINDOW_WIDTH       = 500;
const int WINDOW_HEIGHT      = 500;

void pancake_setup(std::vector<sf::RectangleShape> &, const int);
void update_spatula(sf::RectangleShape &,
                    const std::vector<sf::RectangleShape> &,
                    const sf::RenderWindow &,
                    bool &, int &);
void flip(std::vector<sf::RectangleShape> &, const int);
void randomize(std::vector<sf::RectangleShape> &);

int main()
{
    std::srand(time(NULL));
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "Pancake Flipping");

    window.setFramerateLimit(60);

    bool in_focus = true;
    bool left_bracket_pressed = false;
    bool right_bracket_pressed = false;
    bool r_key_pressed = false;

    int num_pancakes = MAX_PANCAKES / 2;
    std::vector<sf::RectangleShape> pancakes;
    pancake_setup(pancakes, num_pancakes);

    sf::RectangleShape spatula(sf::Vector2f(WINDOW_WIDTH - 30, 10));
    spatula.setFillColor(sf::Color::Red);
    spatula.setOrigin(spatula.getSize().x / 2,
                      spatula.getSize().y / 2);
    spatula.setPosition(WINDOW_WIDTH / 2,
                        WINDOW_HEIGHT / 2);

    bool draw_spatula = true, clicking = false;
    int pancake_index = 0;
    
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed ||
                (in_focus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
            {
                window.close();
            }

            //Adjust window focus if changed.
            if (in_focus && e.type == sf::Event::LostFocus)
                in_focus = false;
            else if (!in_focus && e.type == sf::Event::GainedFocus)
                in_focus = true;
        }

        if (in_focus && window.isOpen())
        {
            //Check if pancakes are reset.
            if (!r_key_pressed &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    randomize(pancakes);
                else
                    pancake_setup(pancakes, num_pancakes);

                r_key_pressed = true;
            }

            else if (r_key_pressed &&
                     !sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                r_key_pressed = false;
            }
                

            //Check left bracket.
            if (!left_bracket_pressed &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket))
            {
                if (num_pancakes > MIN_PANCAKES)
                {
                    --num_pancakes;
                    pancake_setup(pancakes, num_pancakes);
                }
                left_bracket_pressed = true;
            }
            else if (left_bracket_pressed &&
                     !sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket))
            {
                left_bracket_pressed = false;
            }

            //Check right bracket.
            if (!right_bracket_pressed &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket))
            {
                if (num_pancakes < MAX_PANCAKES)
                {
                    ++num_pancakes;
                    pancake_setup(pancakes, num_pancakes);
                }
                right_bracket_pressed = true;
            }
            else if (right_bracket_pressed &&
                     !sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket))
            {
                right_bracket_pressed = false;
            }
            
            //Check for hovering and clicks.
            update_spatula(spatula, pancakes, window,
                           draw_spatula, pancake_index);
            if (draw_spatula)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicking)
                {
                    clicking = true;
                    flip(pancakes, pancake_index);
                }
            }

            //Update the clicking check boolean.
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicking)
                clicking = true;
            else if (clicking && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
                clicking = false;

            //Draw objects to the window.
            window.clear();

            int n = pancakes.size();
            for (int i = 0; i < n; ++i)
                window.draw(pancakes.at(i));

            if (draw_spatula)
                window.draw(spatula);
            
            window.display();
        }
    }
    
    return 0;
}


void pancake_setup(std::vector<sf::RectangleShape> & pancakes, const int num_pancakes)
{
    if (pancakes.size() > 0)
        pancakes.clear();
    for (int i = 0; i < num_pancakes; ++i)
    {
        pancakes.push_back(sf::RectangleShape(sf::Vector2f(100 + (PANCAKE_WIDTH_MULT * i),
                                                           PANCAKE_HEIGHT)));
        pancakes.back().setFillColor(sf::Color::Yellow);
        pancakes.back().setOrigin(pancakes.back().getSize().x / 2,
                                  pancakes.back().getSize().y / 2);
        pancakes.back().setPosition(WINDOW_WIDTH / 2,
                                    WINDOW_HEIGHT / 2 +
                                    ((num_pancakes / 2 * -1) + i) * 30);
    }
    
    return;
}


void update_spatula(sf::RectangleShape & spatula,
                    const std::vector<sf::RectangleShape> & pancakes,
                    const sf::RenderWindow & window,
                    bool & draw_spatula,
                    int & pancake_index)
{
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

    sf::FloatRect rect;
    rect.width = 100 + (MAX_PANCAKES * PANCAKE_WIDTH_MULT);
    rect.height = PANCAKE_HEIGHT;
    rect.left = pancakes.front().getPosition().x -
        (pancakes.front().getSize().x / 2);
   
    draw_spatula = false;
    int n = pancakes.size();
    for (int i = 0; i < n; ++i)
    {
        rect.top = pancakes.at(i).getPosition().y +
            (pancakes.at(i).getSize().y / 2);
        if (rect.contains(mouse_pos.x, mouse_pos.y))
        {
            draw_spatula = true;
            spatula.setPosition(spatula.getPosition().x,
                                rect.top + (PANCAKE_HEIGHT / 2));
            pancake_index = i;
            break;
        }
    }
    
    return;
}


void flip(std::vector<sf::RectangleShape> & pancakes, const int pancake_index)
{
    if (pancake_index != 0)
    {
        float temp;
        int n = (pancake_index + 1) / 2;
        for (int i = 0; i < n; ++i)
        {
            temp = pancakes.at(i).getSize().x;
            pancakes.at(i).setSize(sf::Vector2f(pancakes.at(pancake_index - i).getSize().x,
                                                PANCAKE_HEIGHT));
            pancakes.at(pancake_index - i).setSize(sf::Vector2f(temp, PANCAKE_HEIGHT));

            //Reset origins.
            pancakes.at(i).setOrigin(pancakes.at(i).getSize().x / 2,
                                     pancakes.at(i).getSize().y / 2);
            pancakes.at(pancake_index - i).setOrigin(pancakes.at(pancake_index - i).getSize().x / 2,
                                                     pancakes.at(pancake_index - i).getSize().y / 2);
        }
    }
    
    return;
}


void randomize(std::vector<sf::RectangleShape> & pancakes)
{
    std::vector<sf::Vector2f> size_vects;
    int n = pancakes.size();
    for (int i = 0; i < n; ++i)
        size_vects.push_back(pancakes.at(i).getSize());

    std::random_shuffle(size_vects.begin(), size_vects.end());

    for (int i = 0; i < n; ++i)
    {
        pancakes.at(i).setSize(size_vects.at(i));
        pancakes.at(i).setOrigin(pancakes.at(i).getSize().x / 2,
                                     pancakes.at(i).getSize().y / 2);
    }
    
    return;
}
