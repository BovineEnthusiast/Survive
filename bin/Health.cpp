#include "Health.h"

Health::Health(sf::Texture* pTexture)
	:pTexture_(pTexture)
{
	sprite_.setTexture(*pTexture_);
	sprite_.setOrigin(14.0f, 11.0f);
}

void Health::update(const sf::Time& dT)
{
	sprite_.setPosition(positionGlobal_);
}

//Getters
 sf::Sprite Health::getSprite() const { return sprite_; }
 sf::Clock Health::getDurationClock() const { return durationClock_; }
 bool Health::isCollected() const { return collected_; }

//Setters
 void Health::setCollected(const bool collected) { collected_ = collected; }