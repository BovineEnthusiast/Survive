#include "Tile.h"
Tile::Tile(){}
Tile::Tile(sf::Texture* pTexture, const sf::IntRect& intRect, const std::string& type)
	: type_(type), pTexture_(pTexture)
{
    sprite_.setTexture(*pTexture_);
    sprite_.setTextureRect(intRect);
}

//Getters
sf::Sprite Tile::getSprite() {return sprite_;}
std::string Tile::getType() {return type_;}
bool Tile::hasItem() const { return hasItem_; }
//Setters
void Tile::setSpritePos(const sf::Vector2f& pos) {sprite_.setPosition(pos);}
void Tile::setHasItem(bool hasItem) { hasItem_ = hasItem; }
