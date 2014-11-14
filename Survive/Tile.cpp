#include "Tile.h"
Tile::Tile(){}
Tile::Tile(const sf::Texture& texture, const sf::IntRect& intRect, const std::string& type)
: type_(type)
{
    _sprite.setTexture(texture);
    _sprite.setTextureRect(intRect);
}

//Getters
sf::Sprite Tile::getSprite() {return _sprite;}
std::string Tile::getType() {return type_;}

//Setters
void Tile::setSpritePos(const sf::Vector2f& pos) {_sprite.setPosition(pos);}