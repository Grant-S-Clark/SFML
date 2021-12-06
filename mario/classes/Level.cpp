#include "Level.h"
#include <iostream>
/*
  This constructor will be reading from level.txt to create an array.
  The height and width are already set within, but if deleted,
  width is 204 and height is 14. Remember that each line of the text file has a newline
  character ('n\') at the end of it. Create a 2 dimensional dynamic array to store each tile,
  and determine the tile type using the character provided within the text file at that location.
  You will create 2 of them, one that has the default tile states stored so the game can
  be reset on the fly.
*/
Level::Level(const int gameTileSize)
{
    defaultEntities = std::vector<Entity*>();
    entities = std::vector<Entity*>();

    tileSize = gameTileSize;
    width = 204;
    height = 14;

    //Allocate space for all tiles in the array.
    tiles = new Tile* [height];
    for (int i = 0; i < height; i++)
        *(tiles + i) = new Tile[width];

    file.open("level.txt", std::ios::in);
    char fileChar;

    /*
      Read through the file character by character. If you find a 'P' or 'G',
      push it onto the defaultEntities vector. Also add type 'f' and 'F' for the flagpole so
      collisisons with it can be checked in Game.cpp. Call makeTile with a '-' character for 
      these entities as the array of tiles still needs empty space where these entitiy will
      be placed.
    */
    for(int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            file.get(fileChar);
            
            if (fileChar == 'P' || fileChar == 'G' || fileChar == 'K')
            {
                switch (fileChar)
                {
                    case 'P':
                        defaultEntities.push_back(new Player(tileSize, j * tileSize, i * tileSize));
                        break;
                    case 'G':
                        defaultEntities.push_back(new Goomba(tileSize, j * tileSize, i * tileSize));
                        break;
                }

                //Create Blank space where the entity will be placed.
                (*(tiles + i) + j)->makeTile('-', tileSize, j * tileSize, i * tileSize);
            }

            else
                (*(tiles + i) + j)->makeTile(fileChar, tileSize, j * tileSize, i * tileSize);
			
        }
    file.close();

    //Call resetEntities to set the entities vector.
    resetEntities();

    return;
}


//Delete all data allocated by the array and vectors here.
Level::~Level()
{
    for (int i = 0; i < height; i++)
        delete[] *(tiles + i);
    delete[] tiles;

    for (int i = 0; i < defaultEntities.size(); i++)
        delete defaultEntities.at(i);

    for (int i = 0; i < entities.size(); i++)
        delete entities.at(i);

    return;
}


/*
  Loops through the tiles array and draws it to the screen if it is in view.
  Do not draw open space tiles, as they are blank to begin with so drawing them
  is not necessary.
*/
void Level::drawLevel(sf::RenderWindow* window, sf::View* view)
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (isInView(view, (*(tiles + i) + j)) &&
                (*(tiles + i) + j)->getType() != '-' &&
                (*(tiles + i) + j)->getType() != '\n')
            {
                window->draw(*(*(tiles + i) + j));
            }
        }
			
    return;
}

/*
  Return true if the rectangle shape sent here is within view.
  Note that Tile objects derive from RectangleShape so you can send
  tiles directly to this function instead of a RectangleShape.
*/
bool Level::isInView(sf::View* view, sf::RectangleShape* tile)
{
    sf::FloatRect viewRect;
    viewRect.left = view->getCenter().x - (view->getSize().x / 2);
    viewRect.top = view->getCenter().y - (view->getSize().y / 2);
    viewRect.width = view->getSize().x;
    viewRect.height = view->getSize().y;

    return tile->getGlobalBounds().intersects(viewRect);
}


/*
  Returns a vector of sf::FloatRect objects. These float rects are all from every tile
  that is in view and also collidable such as the floor and bricks, but does not include
  empty space like '-'.
*/
std::vector<sf::FloatRect> Level::getLevelBounds(sf::View* view)
{
    std::vector<sf::FloatRect> rects;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (isInView(view, (*(tiles + i) + j)) &&
                (*(tiles + i) + j)->getType() != '-' &&
                (*(tiles + i) + j)->getType() != '\n')
            {
                rects.push_back((*(tiles + i) + j)->getGlobalBounds());
            }
        }

    return rects;
}


//Returns a pointer to the vector so changes made to it in Game.cpp will
//be applied to this vector as well.
std::vector<Entity*>* Level::getEntities()
{
    return &entities;
}


//Resets the entities vector to have objects equal to the defaultEntities vector.
void Level::resetEntities()
{
    //Deallocate entities withing vector and clear it.
    for (int i = 0; i < entities.size(); i++)
        delete entities.at(i);
    entities.clear();

    /*
      Fill entities with exact copies of the default entities. Note that you cannot
      use "entities = defaultEntities;" as they are pointers so they would be the 
      same objects and not copies. Grab information from the defaultEntities and 
      allocate a new entity within the entities vector which uses information within the
      defaultEntities to set the positions of the new entity.
    */
    for (int i = 0; i < defaultEntities.size(); i++)
    {
        switch (defaultEntities.at(i)->getType())
        {
            case 'P':
                entities.push_back(new Player(tileSize, defaultEntities.at(i)->getPosition().x,
                                              defaultEntities.at(i)->getPosition().y - 50));
                break;
            case 'G':
                entities.push_back(new Goomba(tileSize, defaultEntities.at(i)->getPosition().x,
                                              defaultEntities.at(i)->getPosition().y - 50));
                break;
        }
    }

    return;
}
