#pragma once
#include <SFML/Graphics.hpp>

class Tile : public sf::RectangleShape
{
public:
	Tile();
	~Tile();

	void makeTile(const char, const int, const int, const int);
	void setTileColor();
	char getType();
private:

	char type;
	/*
	Types:
		X = Ground
		B = Brick
		< = Pipe Top Left
		> = Pipe Top right
		[ = Pipe Body Left
		] = Pipe Body Right
		Q = Question Block (Item)
		q = Question Bock (No Item)
		G = Goomba
		P = Player
		F = Flagpole (top)
		f = Flagpole
		- = Sky/Empty
		\n = Sky/Empty
	*/

};

