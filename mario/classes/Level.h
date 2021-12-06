#pragma once
#include <fstream>
#include "Player.h"
#include "Goomba.h"

class Level
{
public:
	Level(const int);
	~Level();

	void drawLevel(sf::RenderWindow*, sf::View*);
	bool isInView(sf::View*, sf::RectangleShape*);
	std::vector<sf::FloatRect> getLevelBounds(sf::View*);
	std::vector<Entity*>* getEntities();
	void resetEntities();
private:
	int width, height, tileSize;
	Tile** tiles;
	std::ifstream file;
	std::vector<Entity*> entities, defaultEntities;
};

