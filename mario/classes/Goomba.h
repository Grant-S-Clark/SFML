#ifndef GOOMBA_H
#define GOOMBA_H

#include "Entity.h"

class Goomba : public Entity
{
public:
	Goomba(const int, const int, const int);
	~Goomba();
	
	void adjust(std::vector<sf::FloatRect>*) override;
	void levelCollide(Side, sf::FloatRect*) override;

private:
	int points;
};

#endif
