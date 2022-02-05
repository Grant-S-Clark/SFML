#include "Button.h"


void Button::set_text_font(const sf::Font & font)
{
    if (text_ == nullptr)
        text_ = new sf::Text;
    text_->setFont(font);

    adjust_text();
        
    return;
}


void Button::set_text_char_size(const int char_size)
{
    if (text_ == nullptr)
        throw NoFontError();
    text_->setCharacterSize(char_size);

    adjust_text();
        
    return;
}


void Button::set_text_color(const sf::Color & color)
{
    if (text_ == nullptr)
        throw NoFontError();

    text_fill_ = color;
    t_text_fill_ = color;
    t_text_fill_.a = 125;

    text_->setFillColor(text_fill_);

    return;
}


void Button::set_text_string(const char * str)
{
    if (text_ == nullptr)
        throw NoFontError();

    text_->setString(str);
        
    adjust_text();
    
    return;
}


void Button::setFillColor(const sf::Color & color)
{
    fill_ = color;
    t_fill_ = color;
    t_fill_.a = 125;

    sf::RectangleShape::setFillColor(fill_);

    return;
}


void Button::setOutlineColor(const sf::Color & color)
{
    outline_ = color;
    t_outline_ = color;
    t_outline_.a = 125;

    sf::RectangleShape::setOutlineColor(outline_);

    return;
}


void Button::update(const sf::RenderWindow & window)
{
    if (text_ != nullptr &&
        getPosition() != text_->getPosition())
    {
        adjust_text();
    }

    //Handle transparency here.
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
    {
        if (getFillColor() != fill_)
        {
            sf::RectangleShape::setFillColor(fill_);
            text_->setFillColor(text_fill_);
        }
    }

    else if (getFillColor() != t_fill_)
    {
        sf::RectangleShape::setFillColor(t_fill_);
        text_->setFillColor(t_text_fill_);
    }
    
    return;
}


bool Button::is_clicked()
{
    if (getFillColor() == t_fill_)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicking)
        {
            clicking = true;
            return true;
        }
    }

    //Update the clicking check boolean.
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicking)
        clicking = true;
    else if (clicking && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        clicking = false;

    return false;
}
