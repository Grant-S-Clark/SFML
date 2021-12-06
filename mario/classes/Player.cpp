#include "Player.h"

//SEND ACTUAL POSITION, NOT THE i OR j INDEXES
Player::Player(const int tileSize, const int x, const int y) : Entity()
{
	//Pass the player character directly to the makeTile function.
	makeTile('P', tileSize, x, y);

	jumpKeyPressed = false;
	jumping = false;
	frames = 0;

	return;
}


Player::~Player()
{
	return;
}


void Player::adjust(std::vector<sf::FloatRect>* levelTiles)
{
	getInput();

	//Checks collisions 4 times inbetween movements to avoid
	//incorrect collision detection when moving too fast.
	for (int i = 0; i < 2; i++)
	{
		checkLevelCollision(levelTiles);
		move(getVX() / 2,getVY() / 2);
	}

	//Max downward speed.
	if (getVY() > 4)
		setVY(4);

	if (abs(getVY()) > 0)
		isInAir() = true;
	else
		isInAir() = false;

	return;
}


void Player::getInput()
{
	//move left.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		setVX(getVX() - .15);

		//velocity cap, different cap if running or not.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (getVX() < -4)
				setVX(-4);
		}
			
		else if (getVX() < -2.5)
			setVX(-2.5);
	}

	//move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		setVX(getVX() + .15);

		//velocity cap, different cap if running or not.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (getVX() > 4)
				setVX(4);
		}
			
		else if (getVX() > 2.5)
			setVX(2.5);
	}

	//no left/right movement keys pressed, slow the player's movement.
	else
	{
		//moving right
		if (getVX() > 0)
		{
			if (getVX() > .1)
				setVX(getVX() - .05);
			else
				setVX(0);
		}

		//moving left
		else
		{
			if (getVX() < -.1)
				setVX(getVX() + .05);
			else
				setVX(0);
		}
	}

	//cant hold jump. (no b-hopping). also allows for different jump heights based on
	//space press length with the jumping boolean.
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		frames = 0;
		jumping = false;

		if (!isInAir())
			jumpKeyPressed = false;
	}

	//jump (REMEMBER TO WORK IN THE FACT IF YOU TAP IT IS A SMALL JUMP,
	//BUT IF YOU HOLD IT IS A BIG JUMP.)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !isInAir() && !jumpKeyPressed)
	{
		jumping = true;
		jumpKeyPressed = true;
		setVY(-2);
	}

	//Frames is used to measure time without a clock object.
	if (jumping && isInAir() && frames <= 20)
	{
		frames++;
		setVY(-2);
	}

	return;
}


