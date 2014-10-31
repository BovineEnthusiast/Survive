#ifndef LEVEL_H
#define	LEVEL_H
#include <memory>
class Level 
{
public:
    Level();
    Level(const Level& orig);
    virtual ~Level();
    
    void update();
    void generateLevel(); //Fills the tiles vector with, well... tiles
private:
    sf::Texture tileSpriteSheet;
    std::map<std::string, sf::IntRect> tileSprites;
    std::vector<std::vector<Tile>> tiles;
};

#endif

