#include "Tile.h"
Tile::Tile(){}
Tile::Tile(const sf::Texture& texture, const sf::IntRect& intRect, const std::string& type)
: type_(type)
{
    sprite_.setTexture(texture);
    sprite_.setTextureRect(intRect);
    
}

//Getters
sf::Sprite Tile::getSprite() {return sprite_;}
std::string Tile::getType() {return type_;}

//Setters
void Tile::setSpritePos(const sf::Vector2f& pos) {sprite_.setPosition(pos);}