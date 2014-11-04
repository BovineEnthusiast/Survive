#ifndef TILE_H
#define	TILE_H
#include <SFML/Graphics.hpp>
class Tile 
{
public:
    Tile();
    Tile(const sf::Texture&, const sf::IntRect&);
    //Getters
    sf::Sprite getSprite();
    
    //Setters
    void setSpritePos(const sf::Vector2f&);
    
    void setTileTexture();
    void setTileTextureRect();
private:
    sf::Sprite _sprite;

};

#endif

