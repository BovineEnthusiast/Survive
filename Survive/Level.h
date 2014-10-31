#ifndef LEVEL_H
#define	LEVEL_H
#include <memory>
class Level 
{
public:
    Level();
    
    void update();
    void generateLevel(const int width, const int height); //Fills the tiles vector with, well... tiles
private:
    sf::Texture _tileSpriteSheet;
    
    //A map of IntRect locations on the sprite that take a name key.
    //i.e setting a tile sprite:
    //sprite.setTextureRect(tileSprites["grass"])
    std::map<std::string, sf::IntRect> _tileSprites; 
    std::vector<std::vector<Tile>> _tiles;
};

#endif

