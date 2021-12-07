#include "Game.h"


Game::Game()
{
    winX = 512;
    winY = 240;
    tileSize = 14;

    //Allocate space for a level object on the heap.
    level = new Level(tileSize);

    //Capture the entities vector from the level.
    entities = level->getEntities();

    //Create a view that is the same size as the window.
    view = new sf::View(sf::Vector2f(winX/2, winY/2), sf::Vector2f(winX, winY));

    return;
}


Game::~Game()
{
    //Delete the dynamically allocated level object.
    delete level;

    return;
}

//Return window width.
int Game::getWinX()
{
    return winX;
}


//Return window height.
int Game::getWinY()
{
    return winY;
}

/*
  Captures a pointer to an sf::RenderWindow so the window->isOpen()
  loop can be in the runGame() function.
  Must call before calling runGame() function.
*/
void Game::getWindowPtr(sf::RenderWindow* winPtr)
{
    window = winPtr;

    return;
}


//Houses the window->isOpen() loop and performs all game logic.
void Game::runGame()
{
    window->setView(*view);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window->close();
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            level->resetEntities();
            view->setCenter(sf::Vector2f(winX / 2, winY / 2));
            window->setView(*view);
        }

        //Clear levelTiles and grab the level bounds from
        //the game's level object pointer.
        levelTiles.clear();
        levelTiles = level->getLevelBounds(view);

        /*
          Loop through all entities in the entities vector. Will need to use
          downcasting to call class specific virtual function defenitions such as
          adjust(). 
          This loop will apply gravity and check collisions between other
          entities and the level bounds which were captured above. 
          If an entity collides with another entity in a way that kills it, set isDead()
          to true and have it removed in the next frame (next time loop is run).
          Such collisions include if mario touches a goomba from the side, or if
          a goomba walks off the left side of the screen, etc.
          Remember that these entities are dynamically allocated so they will have to have
          delete called on them before the object is removed from the vector.
          Only apply position updates and collision checks on entities that are
          on the screen.
          Also display that the player has "Won" if they collide with a flagpole (type 'f' or 'F')
          entity.
        */
        for (int i = 0; i < entities->size(); i++)
        {
            switch (entities->at(i)->getType())
            {
                case 'P':
                    //if player is dead.
                    if (entities->at(i)->isDead())
                    {
                        delete entities->at(i);
                        entities->erase(entities->begin() + i);
                        i--;
                    }

                    else
                    {
                        levelTiles.push_back(getLeftPlayerBarrier());
                        static_cast<Player*>(entities->at(i))->adjust(&levelTiles);
                        if (adjustView(static_cast<Player*>(entities->at(i))))
                            window->setView(*view);
                        levelTiles.pop_back();

                        applyGravity(entities->at(i));
                    }
				
                    break;
                case 'G':
                    //delete if off screen or dead.
                    if (entities->at(i)->getPosition().x < view->getCenter().x - view->getSize().x / 2 ||
                        entities->at(i)->getPosition().y > view->getCenter().y + view->getSize().y / 2 || 
                        entities->at(i)->isDead())
                    {
                        delete entities->at(i);
                        entities->erase(entities->begin() + i);
                        i--;
                    }

                    //if not dead.
                    else if (level->isInView(view, entities->at(i)))
                    {
                        static_cast<Goomba*>(entities->at(i))->adjust(&levelTiles);
                        applyGravity(entities->at(i));
                    }

                    break;
            }

            //Check collisions between entities.
            entityCollisions();
        }

        window->clear();
        level->drawLevel(window, view);
        for (int i = 0; i < entities->size(); i++)
            window->draw(*entities->at(i));
        window->display();

    }

    return;
}


//Increase vertical velocity of an entity.
void Game::applyGravity(Entity* e)
{
    e->setVY(e->getVY() + .105);

    return;
}


/*
  Construct and return an sf::FloatRect that acts as a bounding box on the left
  side of the screen that is only used to make sure mario does not go off of
  the left side of the screen. Also make sure that goombas cannot collide with it,
  as they can move off of the left side of the screen, which also kills them.
*/
sf::FloatRect Game::getLeftPlayerBarrier()
{
    sf::FloatRect leftWall;
    leftWall.left = view->getCenter().x - (view->getSize().x / 2) - 10;
    leftWall.top = view->getCenter().y - view->getCenter().y;
    leftWall.width = 10;
    leftWall.height = view->getSize().x;

    return leftWall;
}


/*
//If the player's x-position goes over the view's horizontal center, adjust
the view so mario is now at its center again.
Returns true if the view was moved and needs to be reapplied to the window,
as it does not need to be reapplied if the view has not changed positions.
*/
bool Game::adjustView(Player* player)
{
    if ((player->getPosition().x - tileSize) > view->getCenter().x)
    {
        view->move((player->getPosition().x - tileSize) - view->getCenter().x, 0);
        return true;
    }

    return false;
}


void Game::entityCollisions()
{
    for (int i = 0; i < entities->size(); i++)
    {
        //Dont have to check if on screen as the view always follows the player.
        if (entities->at(i)->getType() == 'P')
        {
            for (int j = 0; j < entities->size(); j++)
            {
                //dont check collisions with itself, or collisions with
                //objects out of view.
                if (j != i && level->isInView(view, entities->at(j)))
                {
                    if (entities->at(j)->getType() == 'G')
                    {
                        //Creates specially sized collision boxes.
                        entities->at(i)->rotateBox(b);
                        entities->at(i)->getBound()->height = 3;
                        entities->at(i)->getBound()->top = entities->at(i)->getBound()->top - 3;
                        entities->at(j)->rotateBox(t);

                        //if bottom of player collides with top of goomba.
                        if (entities->at(i)->getBound()->intersects(*entities->at(j)->getBound()))
                        {
                            //VY boost from goomba stomp.
                            entities->at(i)->setVY(-1.5);
                            entities->at(j)->isDead() = true;
                        }
                        //if the player is touching the goomba.
                        else if (entities->at(i)->getGlobalBounds().intersects(entities->at(j)->getGlobalBounds()))
                        {
                            entities->at(i)->isDead() = true;
                        }
                    }
                }
            }
        }

        //If it is a goomba and is in view. This will only
        //be utilized for goomba collisions with other enemies.
        if (entities->at(i)->getType() == 'G' &&
            level->isInView(view, entities->at(i)))
        {
            //dont check collisions with itself, or collisions with
            //objects out of view.
            for (int j = 0; j < entities->size(); j++)
            {
                if (j != i && level->isInView(view, entities->at(j)) &&
                    (entities->at(j)->getType() == 'G' || entities->at(j)->getType() == 'K'))
                {
                    if (entities->at(i)->getGlobalBounds().intersects(entities->at(j)->getGlobalBounds()))
                    {
                        //swaps each goomba's VX value along with moving them once to avoid clipping.
                        entities->at(i)->setVX(entities->at(i)->getVX() * -1);
                        entities->at(i)->move(entities->at(i)->getVX(), 0);
                        entities->at(j)->setVX(entities->at(j)->getVX() * -1);
                        entities->at(j)->move(entities->at(j)->getVX(), 0);
                    }
                }
            }
        }
    }

    return;
}
