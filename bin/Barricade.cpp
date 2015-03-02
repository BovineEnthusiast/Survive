#include "Barricade.h"
#include <iostream>

Barricade::Barricade(const sf::Vector2f& position, sf::Texture* pTexture)
	:pTexture_(pTexture)
{
  health_ = 100;
  positionGlobal_ = position;
  sprite_.setTexture(*pTexture_);
  sprite_.setOrigin(16.0f, 16.0f);
}

void Barricade::update(const sf::Time& dT)
{
  if(dead_)
    safeToDelete_ = true;
  sprite_.setPosition(positionGlobal_);
  if(health_ <= 0)
    dead_ = true;
}

//Getters
bool Barricade::isDead() const { return dead_; }
int Barricade::getHealth() const { return health_; }
bool Barricade::isSafeToDelete() const { return safeToDelete_; }
sf::Sprite Barricade::getSprite() const { return sprite_; }

//Setters
void Barricade::setHealth(int health) { health_ = health;}






