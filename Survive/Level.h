#ifndef LEVEL_H
#define	LEVEL_H
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Tile.h"
class Level 
{
public:
    Level();
    
    void update();
    void generateLevel(const int width, const int height); //Fills the tiles vector with procedural tiles
    std::vector< std::vector<Tile> > tiles;

    //Getters
    //Setters
private:
    sf::Texture _tileSpriteSheet;
    
    //A map of IntRect locations on the sprite that take a name key.
    std::map<std::string, sf::IntRect> _tileSprites; 
};

#endif

