#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <time.h>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp> //faster drawing of the particle array.

#include "Button.h" //My button class.

enum Type {
    None = 0,
    Sand,
    Water,
    Wood
};

struct Particle
{
    Type type;
    bool checked;
};

struct IntPair
{
    int x;
    int y;
};

class Simulator
{
public:
    static void get_window_ptr(sf::RenderWindow * const &);
    
    Simulator();
    ~Simulator();
    
    void update();
    void draw_simulator();
private:
    void resize_particles(const int, const int);
    void reset_particles();
    void mouse_input();
    void place_particles(const int, const int, const bool);
    void keyboard_input();
    bool valid_index(const int, const int) const;
    bool swappable(const Type&, const Type&) const;
    void swap(const int, const int, const int, const int);

    sf::RectangleShape outline;
    Particle** particles;

    int width;
    int height;
    int scale_factor;
    int placement_size;
    int rand_1;
    bool left_bracket_pressed;
    bool right_bracket_pressed;
    Type selected_type;

    static sf::RenderWindow* window;

    static std::unordered_map<Type, std::vector<IntPair>> check_pairs;
};

#endif
