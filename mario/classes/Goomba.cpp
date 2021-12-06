#include "Goomba.h"

Goomba::Goomba(const int tileSize, const int x, const int y) : Entity()
{
	//the "x - 1" is to offset them from the ground so collision works
	//correctly right off the bat.
	makeTile('G', tileSize, x - 1, y);
	setVX(-1);

	points = 100;

	return;
}


Goomba::~Goomba()
{
	return;
}


void Goomba::adjust(std::vector<sf::FloatRect>* levelTiles)
{
	for (int i = 0; i < 4; i++)
	{
		checkLevelCollision(levelTiles);
		move(getVX() / 4, getVY() / 4);
	}
	return;
}


void Goomba::levelCollide(Side s, sf::FloatRect* tile)
{
	switch (s)
	{
	case l:
		setVX(1);

		if (tile->intersects(getGlobalBounds()))
			setPosition(tile->left + (tile->width * 1.5), getPosition().y);

		break;
	case r:
		setVX(-1);

		if (tile->intersects(getGlobalBounds()))
			setPosition(tile->left - (tile->width / 2), getPosition().y);

		break;
	case b:
		if (getVY() > 0)
			setVY(0);

		if (tile->intersects(getGlobalBounds()))
			setPosition(getPosition().x, tile->top - (tile->height / 2));

		break;
	case t:
		if (getVY() < 0)
			setVY(0);

		if (tile->intersects(getGlobalBounds()))
			setPosition(getPosition().x, tile->top + (tile->height * 1.5));

		break;
	}

	return;
}