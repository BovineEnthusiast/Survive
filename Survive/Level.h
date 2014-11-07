#ifndef LEVEL_H
#define	LEVEL_H
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "GameObject.h"
class Level 
{
public:
    Level();
    
    void update(const sf::Time&);
    void generateLevel(const int width, const int height); //Fills the tiles vector with procedural tiles
    std::vector< std::vector<Tile> > tiles;

    //Getters
    //Setters
private:
    sf::Texture tileSpriteSheet_;
    
    //A map of IntRect locations on the sprite that take a name key.
    std::map<std::string, sf::IntRect> tileSprites_; 
    
    //All the GameObjects in the level
    std::vector<GameObject> gameObjects_;
    
};

#endif

