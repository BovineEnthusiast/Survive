#include "Tile.h"
#include <iostream>
Tile::Tile(){}
Tile::Tile(sf::Texture* pTexture, const sf::IntRect& intRect, const std::string& type)
	: type_(type), pTexture_(pTexture)
{
	if (type == "rock")
	std::cout << type << std::endl;
    sprite_.setTexture(*pTexture_);
    sprite_.setTextureRect(intRect);
    sprite_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
}

//Getters
sf::Sprite Tile::getSprite() {return sprite_;}
std::string Tile::getType() {return type_;}
bool Tile::hasItem() const { return hasItem_; }
//Setters
void Tile::setSpritePos(const sf::Vector2f& pos) {sprite_.setPosition(pos);}
void Tile::setHasItem(bool hasItem) { hasItem_ = hasItem; }
void Tile::setType(const std::string type) { type_ = type; }