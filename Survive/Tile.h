#ifndef TILE_H
#define	TILE_H
#include <SFML/Graphics.hpp>
class Tile 
{
public:
    Tile();
    Tile(const float);
    //Getters
    sf::Sprite getSprite();
    
    //Setters
    void setTileTexture();
    void setTileTextureRect();
private:
    sf::Sprite _sprite;

};

#endif

