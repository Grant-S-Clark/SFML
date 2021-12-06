#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	Player(const int, const int, const int);
	~Player();

	void adjust(std::vector<sf::FloatRect>*) override;
	void getInput();

private:
	bool jumpKeyPressed, jumping;
	int frames;
};

