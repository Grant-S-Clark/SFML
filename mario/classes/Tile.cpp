#include "Tile.h"

//0 is default size before size is passed.
Tile::Tile() : RectangleShape(sf::Vector2f(0, 0))
{
	return;
}


Tile::~Tile()
{
	return;
}


//int x and int y are the position of the tile itself, NOT the array index.
void Tile::makeTile(const char tileType, const int tileSize, const int x, const int y) 
{
	type = tileType;

	setSize(sf::Vector2f(tileSize, tileSize));
	setTileColor();

	//move the tile into the correct position with the x and y parameters.
	//y is offset by 50 to print at bottom of the sceen.
	
	setPosition(x, y + 50);
	setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);

	return;
}


void Tile::setTileColor()
{
	switch (type)
	{
	//Ground
	case 'X':
		setFillColor(sf::Color(105, 55, 0));
		break;
	//Bricks
	case 'B':
		setFillColor(sf::Color(175, 75, 30));
		break;
	//Pipe (All Parts)
	case '<':
	case '>':
	case '[':
	case ']':
		setFillColor(sf::Color(0, 150, 50));
		break;
	//Question Mark Blocks (With Items and Without).
	case 'Q':
	case 'q':
		setFillColor(sf::Color(255, 150, 0));
		break;
	//Enemies (Will be changed later to G and K for goombas and koopas) << REMOVE.
	case 'E':
		setFillColor(sf::Color(150, 0, 0));
		break;
	//Goomba << DEBUG.
	case 'G':
		setFillColor(sf::Color(150, 0, 0));
		break;
	//Koopa << DEBUG.
	case 'K':
		setFillColor(sf::Color(0, 150, 0));
		break;
	//Player
	case 'P': //
		setFillColor(sf::Color(255, 0, 0));
		break;
	//Flagpole
	case 'F':
		setFillColor(sf::Color(0, 255, 0));
		break;
	//Invisible Block
	case 'I':
		setFillColor(sf::Color(175, 75, 30, 0));
		break;
	case '-':
	case '\n':
		setFillColor(sf::Color(0, 0, 0, 0));
	}

	return;
}


char Tile::getType()
{
	return type;
}