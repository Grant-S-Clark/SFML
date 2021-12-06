#pragma once
#include "Level.h"
#include "Player.h"
#include "Goomba.h"

class Game
{
public:
	Game();
	~Game();

	int getWinX();
	int getWinY();

	void getWindowPtr(sf::RenderWindow*);
	void runGame();
	void applyGravity(Entity*);
	sf::FloatRect getLeftPlayerBarrier();
	bool adjustView(Player*);
	void entityCollisions();
	
private:
	int winX, winY, tileSize;
	sf::RenderWindow *window;
	Level *level;
	sf::View *view;
	std::vector<Entity*>* entities;
	std::vector<sf::FloatRect> levelTiles;
};

