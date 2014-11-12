#include "Tile.h"

Tile::Tile() 
{
}
Tile::Tile(const sf::Texture& texture, const sf::IntRect& intRect)
{
    _sprite.setTexture(texture);
    _sprite.setTextureRect(intRect);
}

//Getters
sf::Sprite Tile::getSprite() {return _sprite;}

//Setters
void Tile::setSpritePos(const sf::Vector2f& pos) {_sprite.setPosition(pos);}