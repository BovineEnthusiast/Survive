#include "Barricade.h"

Barricade::Barricade(const sf::Vector2f& position, sf::Texture* pTexture)
	:pTexture_(pTexture)
{
	positionGlobal_ = position;
	sprite_.setTexture(*pTexture_);
	sprite_.setOrigin(16.0f, 16.0f);
}

void Barricade::update(const sf::Time& dT)
{
	sprite_.setPosition(positionGlobal_);
}

//Getters
bool Barricade::isDead() const { return dead_; }
bool Barricade::isReadyToDelete() const { return readyToDelete_; }
sf::Sprite Barricade::getSprite() const { return sprite_; }