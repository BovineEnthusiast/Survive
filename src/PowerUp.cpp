#include "PowerUp.h"

PowerUp::PowerUp(sf::Texture* pTexture, const std::string& type)
    :pTexture_(pTexture), type_(type)
{
    sprite_.setTexture(*pTexture_);
}

//Getters
sf::Sprite PowerUp::getSprite() const { return sprite_; }

//Setters
