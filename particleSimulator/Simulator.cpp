#include "Simulator.h"

Simulator::Simulator(sf::Vector2u window_size)
    : width((window_size.x - 40) / 4),
      height((window_size.y - 140) / 4)
{
    //Seed the rand table.
    std::srand(time(NULL));

    //Allocate space for an array of particles.
    particles = new Particle *[width];
    for (int i = 0; i < width; i++)
        particles[i] = new Particle[height];

    //Set to blank
    reset_particles();

    //Create the outline for the particle array to distinguish
    //its space limits.
    outline.setSize(sf::Vector2f(width * 4, height * 4));
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(3);
    outline.setOutlineColor(sf::Color::Red);
    outline.setPosition(20, 20);

    //Create the printer object. This object will be
    //used for printing each particle in the particle
    //array.
    printer.setSize(sf::Vector2f(4, 4));

    //Default selected type is set to sand.
    selected_type = Sand;

    //Default placement_size is 0.
    placement_size = 0;

    rand_1 = 1;

    left_bracket_pressed = false;
    right_bracket_pressed = false;

    return;
}


Simulator::~Simulator()
{
    //return the space used by particle array.
    for (int i = 0; i < width; i++)
        delete[] particles[i];
    delete[] particles;

    return;
}


void Simulator::reset_particles()
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            particles[i][j].type = None;
            particles[i][j].checked = false;
        }

    return;
}


void Simulator::update(sf::RenderWindow& window)
{
    //Capture mouse input if mouse is within the outline
    //of the particle array.
    if (outline.getGlobalBounds().contains(
            sf::Mouse::getPosition(window).x,
            sf::Mouse::getPosition(window).y)
        )
    {
        mouse_input(window);
    }
    
    //Capture keyboard input.
    keyboard_input();

    //Update particles.
    for (int y = height - 1; y >= 0; y--)
        for (int x = width - 1; x >= 0; x--)
        {
            if (!particles[x][y].checked && particles[x][y].type != None)
            {
                if (std::rand() % 2 == 0)
                    rand_1 *= -1;

                //Reminder that the Wood type has no update cycles.
                switch (particles[x][y].type)
                {
                    case Sand:
                        if (valid_index(x, y + 1) &&
                            swappable(particles[x][y].type, particles[x][y + 1].type) &&
                            !particles[x][y + 1].checked)
                        {
                            //Swap the particles and set them as checked.
                            swap(x, y, x, y + 1);
                            particles[x][y + 1].checked = true;
                        }
                        //Impliments rand_1 to randomly determine whether
                        //left or right side is checked first.
                        else if (valid_index(x - rand_1, y + 1) &&
                                 swappable(particles[x][y].type, particles[x - rand_1][y + 1].type) &&
                                 !particles[x - rand_1][y + 1].checked)
                        {
                            swap(x, y, x - rand_1, y + 1);
                            particles[x - rand_1][y + 1].checked = true;
                        }
                        else if (valid_index(x + rand_1, y + 1) &&
                                 swappable(particles[x][y].type, particles[x + rand_1][y + 1].type) &&
                                 !particles[x + rand_1][y + 1].checked)
                        {
                            swap(x, y, x + rand_1, y + 1);
                            particles[x + rand_1][y + 1].checked = true;
                        }
                        break;
                    case Water:
                        if (valid_index(x, y + 1) &&
                            swappable(particles[x][y].type, particles[x][y + 1].type) &&
                            !particles[x][y + 1].checked)
                        {
                            //Swap the particles and set them as checked.
                            swap(x, y, x, y + 1);
                            particles[x][y + 1].checked = true;
                        }
                        else if (valid_index(x - rand_1, y + 1) &&
                                 swappable(particles[x][y].type, particles[x - rand_1][y + 1].type) &&
                                 !particles[x - rand_1][y + 1].checked)
                        {
                            swap(x, y, x - rand_1, y + 1);
                            particles[x - rand_1][y + 1].checked = true;
                        }
                        else if (valid_index(x + rand_1, y + 1) &&
                                 swappable(particles[x][y].type, particles[x + rand_1][y + 1].type) &&
                                 !particles[x + rand_1][y + 1].checked)
                        {
                            swap(x, y, x + rand_1, y + 1);
                            particles[x + rand_1][y + 1].checked = true;
                        }
                        else if (valid_index(x - rand_1, y) &&
                                 swappable(particles[x][y].type, particles[x - rand_1][y].type) &&
                                 !particles[x - rand_1][y].checked)
                        {
                            swap(x, y, x - rand_1, y);
                            particles[x - rand_1][y].checked = true;
                        }
                        else if (valid_index(x + rand_1, y) &&
                                 swappable(particles[x][y].type, particles[x + rand_1][y].type) &&
                                 !particles[x + rand_1][y].checked)
                        {
                            swap(x, y, x + rand_1, y);
                            particles[x + rand_1][y].checked = true;
                        }
                        break;
                }
            }
        }

    //Set all particles to unchecked for next iteration.
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            particles[i][j].checked = false;

    return;
}


bool Simulator::valid_index(const int x, const int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return true;
    return false;
}


//Check if one particle type is swappable with another.
bool Simulator::swappable(Type type1, Type type2)
{
    switch (type1)
    {
	case Sand:
            if (type2 == Water || type2 == None)
                return true;
            return false;
	case Water:
            if (type2 == None)
                return true;
            return false;
	case Wood:
            return true;
    }

    //This solely exists so the compiler will stop
    //yelling at me.
    return true;
}


//Swap the values of particles[x1][y1] and particles[x2][y2].
void Simulator::swap(const int x1, const int y1, const int x2, const int y2)
{
    Type temp;
    temp = particles[x1][y1].type;
    particles[x1][y1].type = particles[x2][y2].type;
    particles[x2][y2].type = temp;

    return;
}


//Check for mouse input to determine if to place new particles.
void Simulator::mouse_input(sf::RenderWindow& window)
{
    int xPos, yPos;

    //Check for clicking within the particle array.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        xPos = (sf::Mouse::getPosition(window).x - 20) / 4;
        yPos = (sf::Mouse::getPosition(window).y - 20) / 4;

        place_particles(xPos, yPos, false);
    }

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        xPos = (sf::Mouse::getPosition(window).x - 20) / 4;
        yPos = (sf::Mouse::getPosition(window).y - 20) / 4;

        place_particles(xPos, yPos, true);
    }

    return;
}


//change the type of particle that is at a specific location.
//if remove = true, use Type None instead of the selected type.
void Simulator::place_particles(const int xPos, const int yPos,
                                const bool remove)
{
    for(int x = -placement_size; x <= placement_size / 2; x++)
        for (int y = -placement_size; y <= placement_size / 2; y++)
        {
            if (xPos + x < width &&
                xPos + x > -1 &&
                yPos + y < height &&
                yPos + y > -1)
            {
                if(remove)
                    particles[xPos + x][yPos + y].type = None;
                else if(swappable(selected_type, particles[xPos + x][yPos + y].type))
                    particles[xPos + x][yPos + y].type = selected_type;
            }
        }

    return;
}


/*
  Check for keyboard input. Used for switching particle types and resetting
  they particle array. Also used to change the size of the placement area by
  using the [ and ] keys.
*/
void Simulator::keyboard_input()
{
    //R Key: Reset the particle array.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        reset_particles();

    //Number keys will select different particles.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        selected_type = Sand;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        selected_type = Water;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        selected_type = Wood;

    //Brackets will change particle placement size.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket) &&
        !left_bracket_pressed)
    {
        //Minimum placement size.
        if (placement_size != 0)
            placement_size--;
        left_bracket_pressed = true;
    }

    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket) &&
             left_bracket_pressed)
        left_bracket_pressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) &&
        !right_bracket_pressed)
    {
        //Maximum placement size.
        if (placement_size != 5)
            placement_size++;
        right_bracket_pressed = true;
    }

    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) &&
             right_bracket_pressed)
        right_bracket_pressed = false;

    return;
}


//Draw particle array to the screen.
void Simulator::draw_simulator(sf::RenderWindow& window)
{
    //Draw outline of particle array.
    window.draw(outline);

    //Draw particle array.
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            switch (particles[i][j].type)
            {
                case Sand:
                    printer.setFillColor(sf::Color::Yellow);
                    //Moves 20 pixels for offset from top right of window border.
                    printer.setPosition(20 + (i * 4), 20 + (j * 4));
                    window.draw(printer);
                    break;
                case Water:
                    printer.setFillColor(sf::Color::Blue);
                    printer.setPosition(20 + (i * 4), 20 + (j * 4));
                    window.draw(printer);
                    break;
                case Wood:
                    printer.setFillColor(sf::Color(150, 75, 0)); //Brown
                    printer.setPosition(20 + (i * 4), 20 + (j * 4));
                    window.draw(printer);
                    break;
            }
        }
    }

    return;
}
