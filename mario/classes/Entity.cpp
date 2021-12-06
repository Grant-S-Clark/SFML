#include "Entity.h"

Entity::Entity() : Tile()
{
	vX = 0;
	vY = 0;
	inAir = false;
	dead = false;

	return;
}


Entity::~Entity()
{
	return;
}


//SEND ACTUAL POSITION, NOT THE i OR j INDEXES
void Entity::DEBUG_TILE_CREATION(const int tileSize, const int x, const int y)
{
	makeTile('E', tileSize, x, y);

	return;
}

void Entity::setVX(const double newVX)
{
	vX = newVX;
	return;
}


void Entity::setVY(const double newVY)
{
	vY = newVY;
	return;
}


double Entity::getVX()
{
	return vX;
}


double Entity::getVY()
{
	return vY;
}


bool& Entity::isInAir()
{
	return inAir;
}


bool& Entity::isDead()
{
	return dead;
}


sf::FloatRect* Entity::getBound()
{
	return &boundBox;
}


void Entity::adjust(std::vector<sf::FloatRect>* tiles)
{
	return;
}


void Entity::rotateBox(Side side)
{
	switch (side)
	{
	case l:
		getBound()->top = getGlobalBounds().top;
		getBound()->left = getGlobalBounds().left - 1;
		getBound()->height = getGlobalBounds().height;
		getBound()->width = 1;
		break;
	case r:
		getBound()->top = getGlobalBounds().top;
		getBound()->left = getGlobalBounds().left + getGlobalBounds().width;
		getBound()->height = getGlobalBounds().height;
		getBound()->width = 1;
		break;
	case b:
		getBound()->top = getGlobalBounds().top + getGlobalBounds().height;
		getBound()->left = getGlobalBounds().left;
		getBound()->height = 1;
		getBound()->width = getGlobalBounds().width;
		break;
	case t:
		getBound()->top = getGlobalBounds().top - 1;
		getBound()->left = getGlobalBounds().left;
		getBound()->height = 1;
		getBound()->width = getGlobalBounds().width;
		break;
	}

	return;
}


void Entity::checkLevelCollision(std::vector<sf::FloatRect>* levelTiles)
{
	bool collided;
	for (int i = 0; i < levelTiles->size(); i++)
	{
		collided = false;
		for (int s = 0; s < 4 && !collided; s++)
		{
			rotateBox(Side(s));
			if (getBound()->intersects(levelTiles->at(i)))
			{
				levelCollide(Side(s), &levelTiles->at(i));
				collided = true;
			}
		}
	}

	return;
}


void Entity::levelCollide(Side s, sf::FloatRect* tile)
{
	switch (s)
	{
	case l:
		if(getVX() < 0)
			setVX(0);

		//moves the player out of the colliding object to avoid clipping.
		if (tile->intersects(getGlobalBounds()))
			setPosition(tile->left + (tile->width * 1.5) , getPosition().y);

		break;
	case r:
		if(getVX() > 0)
			setVX(0);

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