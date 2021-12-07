#ifndef SIMULATOR_CLASS
#define SIMULATOR_CLASS

#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>

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

class Simulator
{
public:
	Simulator(sf::Vector2u);
	~Simulator();

	void update(sf::RenderWindow&);
	void draw_simulator(sf::RenderWindow&);
private:
	void reset_particles();
	void mouse_input(sf::RenderWindow&);
	void place_particles(const int, const int, const bool);
	void keyboard_input();
	bool valid_index(const int, const int);
	bool swappable(Type, Type);
	void swap(const int, const int, const int, const int);

	sf::RectangleShape outline;
	sf::RectangleShape printer;
	Particle** particles;

	int width;
	int height;
	int placement_size;
	int rand_1;
	bool left_bracket_pressed;
	bool right_bracket_pressed;
	Type selected_type;
};

#endif