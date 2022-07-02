#ifndef SORT_RECT_H
#define SORT_RECT_H

#include <SFML/Graphics.hpp>

class SortRect : public sf::RectangleShape
{
public:
    //The value stored in a SortRect is equal to its height.
    SortRect(int width, int value) :
        sf::RectangleShape(sf::Vector2f(width, value)),
        value_(value)
    {
        setOrigin(getOrigin().x, getOrigin().y + value);
        setFillColor(sf::Color::Green);
        
        return;
    }

    ~SortRect(){}

    void setPosition(const int x_pos)
    {
        sf::RectangleShape::setPosition(x_pos, y_pos_);
    }

    bool operator<(const SortRect & r) const
    {
        return value_ < r.value_;
    }

    bool operator>(const SortRect & r) const
    {
        return value_ > r.value_;
    }

    static void set_y_pos(const int y_pos);
    
private:
    static int y_pos_;
    int value_;
};

#endif
