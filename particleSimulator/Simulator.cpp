#include "Simulator.h"

sf::RenderWindow * Simulator::window = nullptr;

std::unordered_map<Type, std::vector<IntPair>> Simulator::check_pairs =
{
    {Sand, {{0, 1},
            {-1, 1},
            {1, 1}}
    },
    
    {Water, {{0, 1},
             {-1, 1},
             {1, 1},
             {-1, 0},
             {1, 0}}
    },
    {Wood, {}}
};

//YOU MUST CALL THIS BEFORE CREATING A SIMULATOR OBJECTS.
void Simulator::get_window_ptr(sf::RenderWindow * const & w_ptr)
{
    window = w_ptr;

    return;
}


Simulator::Simulator()
    : width((window->getSize().x - 40) / 4),
      height((window->getSize().y - 180) / 4),
      scale_factor(4)
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
    outline.setSize(sf::Vector2f(width * scale_factor,
                                 height * scale_factor));
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(3);
    outline.setOutlineColor(sf::Color::Red);
    outline.setPosition(20, 20);

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
    //return the space used by particle array to the computer.
    for (int i = 0; i < width; i++)
        delete[] particles[i];
    delete[] particles;

    return;
}


void Simulator::update()
{
    //Capture mouse input if mouse is within the outline
    //of the particle array.
    if (outline.getGlobalBounds().contains(
            sf::Mouse::getPosition(*window).x,
            sf::Mouse::getPosition(*window).y)
        )
    {
        mouse_input();
    }
    
    //Capture keyboard input.
    keyboard_input();

    //Update particles.
    std::vector<IntPair>* pairs;
    int loop_size;
    for (int y = height - 1; y >= 0; --y)
        for (int x = width - 1; x >= 0; --x)
        {
            if (particles[x][y].type != None &&
                !particles[x][y].checked)
            {
                if (std::rand() % 2 == 0)
                    rand_1 *= -1;
                pairs = &(check_pairs[particles[x][y].type]);
                loop_size = pairs->size();
                for (int i = 0, n_x, n_y; i < loop_size; ++i)
                {
                    //New x and New y.
                    n_x = x + (pairs->at(i).x * rand_1);
                    n_y = y + pairs->at(i).y;
                    if (valid_index(n_x, n_y))
                    {
                        if (!particles[n_x][n_y].checked &&
                            swappable(particles[x][y].type,
                                      particles[n_x][n_y].type))
                        {
                            swap(x, y, n_x, n_y);
                            particles[n_x][n_y].checked = true;
                            if (particles[x][y].type != None)
                                particles[x][y].checked = true;
                            //If they swapped, break out of the loop.
                            break;
                        }
                    }
                }
            }
        }

    //Set all particles to unchecked for next iteration.
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            particles[i][j].checked = false;

    return;
}


//Draw particle array to the screen.
void Simulator::draw_simulator()
{
    //Draw outline of particle array.
    window->draw(outline);

    //Draw particle array.
    glBegin(GL_POINTS);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (particles[i][j].type != None)
            {
                //Select Color
                switch (particles[i][j].type)
                {
                    case Sand:
                        glColor3ub(255, 255, 0); //YELLOW
                        break;
                    case Water:
                        glColor3ub(0, 0, 255); //BLUE
                        break;
                    case Wood:
                        glColor3ub(150, 75, 0); //BROWN
                        break;
                }

                //Draw particle.
                for (int k = 0; k < scale_factor * scale_factor; ++k)
                    glVertex2i((i * scale_factor) + k % scale_factor,
                               (j * scale_factor) + (k / scale_factor));
            }
        }
    }
    glEnd();

    return;
}


//////////// PRIVATE ////////////

void Simulator::resize_particles(const int new_width,
                                 const int new_height)
{
    Particle** new_arr = new Particle*[new_width];
    for (int i = 0; i < new_width; ++i)
        new_arr[i] = new Particle[new_height];

    //reset new particle array.
    for (int i = 0; i < new_width; i++)
        for (int j = 0; j < new_height; j++)
        {
            new_arr[i][j].type = None;
            new_arr[i][j].checked = false;
        }
    
    //if size increased.
    if (new_width > width)
    {
        //Only increases via doubling.
        for (int i = 0; i < width; ++i)
            for (int j = 0; j < height; ++j)
                new_arr[width + i][height + j] =
                    particles[i][j];
    }

    else
    {
        for (int i = 0; i < new_width; ++i)
            for (int j = 0; j < new_height; ++j)
                new_arr[i][j] = particles[new_width + i][new_height + j];
    }

    //Delete old array and change pointer.
    for (int i = 0; i < width; i++)
        delete[] particles[i];
    delete[] particles;

    particles = new_arr;

    //Change size.
    width = new_width;
    height = new_height;
    
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


bool Simulator::valid_index(const int x, const int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return true;
    return false;
}


//Check if one particle type is swappable with another.
bool Simulator::swappable(const Type & type1,
                          const Type & type2) const
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
void Simulator::swap(const int x1,
                     const int y1,
                     const int x2,
                     const int y2)
{
    Type temp;
    temp = particles[x1][y1].type;
    particles[x1][y1].type = particles[x2][y2].type;
    particles[x2][y2].type = temp;

    return;
}


//Check for mouse input to determine if to place new particles.
void Simulator::mouse_input()
{
    int xPos, yPos;

    //Check for clicking within the particle array.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        xPos = (sf::Mouse::getPosition(*window).x - 20) / scale_factor;
        yPos = (sf::Mouse::getPosition(*window).y - 20) / scale_factor;

        place_particles(xPos, yPos, false);
    }

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        xPos = (sf::Mouse::getPosition(*window).x - 20) / scale_factor;
        yPos = (sf::Mouse::getPosition(*window).y - 20) / scale_factor;

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            //Minimum scale factor.
            if (scale_factor != 1)
            {
                scale_factor /= 2;
                resize_particles(width * 2, height * 2);
            }
        }
        else
        {
            //Minimum placement size.
            if (placement_size != 0)
                placement_size--;
        }
        
        left_bracket_pressed = true;
    }

    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket) &&
             left_bracket_pressed)
        left_bracket_pressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) &&
        !right_bracket_pressed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            //Minimum scale factor.
            if (scale_factor != 16)
            {
                scale_factor *= 2;
                resize_particles(width / 2, height / 2);
            }
        }
        else
        {
            //Maximum placement size.
            if (placement_size != 10)
                placement_size++;
        }
        
        right_bracket_pressed = true;
    }

    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) &&
             right_bracket_pressed)
        right_bracket_pressed = false;

    return;
}


