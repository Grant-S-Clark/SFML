#ifndef ENTITY_H
#define ENTITY_H

#include "Tile.h"
#include <vector>

//Sides of the bounding rectangle.
enum Side { l = 0, r, b, t };

class Entity : public Tile
{
public:
	Entity();
	~Entity();

	void DEBUG_TILE_CREATION(const int, const int, const int);

	void setVX(const double);
	void setVY(const double);
	double getVX();
	double getVY();
	bool& isInAir();
	bool& isDead();

	//Bounding rectangle functions.
	sf::FloatRect* getBound();
	virtual void adjust(std::vector<sf::FloatRect>*);
	void rotateBox(Side);
	void checkLevelCollision(std::vector<sf::FloatRect>*);
	virtual void levelCollide(Side, sf::FloatRect*);

private:
	sf::FloatRect boundBox;
	double vX, vY;
	bool inAir, dead;
};

#endif
