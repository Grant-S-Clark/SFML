#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    //Create game object
    Game game;

    //Create window object. Setting a framerate limit is recommended.
    sf::RenderWindow window(sf::VideoMode(game.getWinX(), game.getWinY()), "Super Mario");
    window.setFramerateLimit(60);

    //Send the window's address to the game object and run the game.
    game.getWindowPtr(&window);
    game.runGame();

    return 0;
}
